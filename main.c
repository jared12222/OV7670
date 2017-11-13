#include ".\LIB\rev_STDIO.h"
#include <stdio.h>
#include <avr/io.h>
#include "lib_OV7670.c"
#include <util/delay.h>

int main(void)
{
    uint8_t chk;
    rev_STDIO_set();
    setup();
    while(1)
    {

        //printf("START %.2X\n",PIND);
    }
}
