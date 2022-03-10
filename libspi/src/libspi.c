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

#include "loragw_hal.h"
#include "loragw_reg.h"
#include "loragw_aux.h"


void usage(void) {
    printf("\nHelp:\n");
    printf(" -h --h print this help\n");
    printf(" -d --device  # setting device path\n\t--device /dev/ttyUSB0\n");
    printf("\n");
}

int main(int argc, char **argv){
    unsigned int arg_u;
    uint8_t clocksource = 0;
    lgw_radio_type_t radio_type = LGW_RADIO_TYPE_SX1250;
    struct lgw_conf_board_s boardconf;
    struct lgw_conf_rxrf_s rfconf;
    uint64_t eui;
    char DEVICE_NAME[128];

    int i;
    /* Parameter parsing */
    int option_index = 0;
    static struct option long_options[] = {
        {"help",0,0,'h'},
        {"device",1,0,'d'},
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

    return 0;
}
