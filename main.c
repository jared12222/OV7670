#include ".\LIB\rev_STDIO.h"
#include <stdio.h>
#include <avr/io.h>
#include "lib_twi_m328p.c"
#include "lib_pwm_m328p.c"
#include <util/delay.h>

#define camAddr_WR 0x42

uint8_t wrReg (uint8_t reg,uint8_t data)
{
    uint8_t chk = 0;
	start();
	chk = write_addr(camAddr_WR,0x18);
    if(chk)
        printf("ERR camID %.2X\n",chk);
	chk = write_data(reg,0x28);
    if(chk)
        printf("ERR reg %.2X\n",chk);
	chk = write_data(data,0x28);
    if(chk)
        printf("ERR data %.2X\n",chk);
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
    return chk;
}

int main(void)
{
    uint8_t chk;
    rev_STDIO_set();
    pwm_ini();
    twi_ini();

    while(1)
    {
        chk = wrReg(0x12, 0x80);
        printf("%X\n",chk);
        _delay_ms(50);
        //printf("START %.2X\n",PIND);
    }
}
