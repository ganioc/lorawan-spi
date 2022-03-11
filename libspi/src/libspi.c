/*

Description: USB SPI lib

*/

/* fix an issue between POSIX and C99 */
#if __STDC_VERSION__ >= 199901L
    #define _XOPEN_SOURCE 600
#else
    #define _XOPEN_SOURCE 500
#endif

#include <stdio.h>
#include <string.h>
#include <getopt.h>     /* getopt_long */
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#include "loragw_hal.h"
#include "loragw_reg.h"
#include "loragw_aux.h"


void usage(void) {
    printf("\nHelp:\n");
    printf(" -h --h print this help\n");
    printf(" -d --device  # setting device path\n\t--device /dev/ttyUSB0\n");
    printf("\n");
}
int open_uart(char* path){
    int fd; /* File descriptor for the port */


    fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
   /*
    * Could not open the port.
    */

        perror("open_port: Unable to open /dev/ttyf1 - ");
    }
    else
        fcntl(fd, F_SETFL, 0);

    return (fd);
}
int config_uart(int fd){
    struct termios tty;
    if(tcgetattr(fd, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all the size bits, then use one of the statements below
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    tty.c_lflag &= ~ICANON; //Disabling Canonical Mode
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    // B0,  B50,  B75,  B110,  B134,  B150,  B200, B300, B600, B1200, B1800, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }


}
int close_uart(int fd){
    return close(fd);

}
int tx_task(int fd){
    unsigned char msg[] = {0x01, 0x02, 0x03};
    int n;

    for(int i=0; i< 10; i++){
        n = write(fd, msg, sizeof(msg));
        printf("Write out: %d\n", n);
        if(n != sizeof(msg)){
            printf("Error sending %n , lost %d\n", 
                sizeof(msg), 
                sizeof(msg)-n);
        }
        sleep(1);
    }
    
    return 0;
}
int rx_task(int fd){
    int n;
    unsigned char buf[1025];
    while(true){
        n=read(fd,buf,1204);
        if(n){
            printf("RX: %d\n", n);
        }else if(n<0){
            printf("Error: receiving error\n");
        }else{
            printf("RX: timeout\n");
        }
    }

    return 0;
}
int main(int argc, char **argv){
    unsigned int arg_u;
    uint8_t clocksource = 0;
    lgw_radio_type_t radio_type = LGW_RADIO_TYPE_SX1250;
    struct lgw_conf_board_s boardconf;
    struct lgw_conf_rxrf_s rfconf;
    uint64_t eui;
    char DEVICE_NAME[128];
    int fd;
    int tx_enabled = 0;
    int rx_enabled = 0;

    int i;
    /* Parameter parsing */
    int option_index = 0;
    static struct option long_options[] = {
        {"help",0,0,'h'},
        {"device",1,0,'d'},
        {"tx",0,0,'t'},
        {"rx",0,0,'r'},
        {0, 0, 0, 0}
    };

    /* parse command line options */
    while ((i = getopt_long(argc, argv, "hd:", long_options, &option_index)) != -1){
        switch (i) {
            case 'h': //printf("help\n");
                    usage();
                    return -1;
                    break;
            case 'd':
                    // printf("device is %s\n", optarg);
                    sprintf(DEVICE_NAME, "%s", optarg);
                    break;
            case 't': tx_enabled = 1;
                    break;
            case 'r': rx_enabled = 1;
                    break;
            default: // printf("Unknown\n");
                    return -1;
                    break;
        }
    }


    // printf("libspi ->\n");
    // parse argv
    // if DEVICE_NAME is empty, exit
    if(strlen(DEVICE_NAME) == 0){
        printf("--device empty\n");
        usage();
        return -1;
    }

    if(tx_enabled && rx_enabled || (!tx_enabled && !rx_enabled)){
        printf("--tx or --rx\n");
        return -1;
    }

    printf("Open uart: %s\n", DEVICE_NAME);
    fd = open_uart(DEVICE_NAME);

    config_uart(fd);

    if(tx_enabled){
        tx_task(fd);
    }else if(rx_enabled){
        rx_task(fd);
    }

    
    printf("Close uart: %d\n", fd);
    close_uart(fd);

    return 0;
}
