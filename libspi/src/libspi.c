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

int main(int argc, char **argv){
    printf("libspi ->\n");

    return 0;
}