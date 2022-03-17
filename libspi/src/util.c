#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>


int print_buffer(char*mark,char*str, int size){
    long ms;
    time_t s;
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s = spec.tv_sec;
    ms = round(spec.tv_nsec/1.0e6);
    if (ms > 999){
        s++;
        ms = 0;
    }

    printf("%s %ld.%03ld: ", mark, (intmax_t)s, ms);
    for(int i=0; i< size; i++){
        printf("0x%02x ", str[i]);
    }
    printf("\n");
    return 0;
}

int print_tx_buffer(char*str, int size){
    return print_buffer("<=",str, size);
}

int print_rx_buffer(char*str, int size){
    return print_buffer("=>",str, size);
}