#include ".\LIB\rev_STDIO.h"
#include <stdio.h>
#include <avr/io.h>
#include "lib_OV7670.c"
#include <util/delay.h>
#include <avr/interrupt.h>

                            // PCLK  -> PD2(INT0)
#define VSYNC 0b00000001    // VSYNC -> PB0
#define HREF  0b00001000    // HREF  -> PD3(INT1)
#define buffer_max   1000
#define row_limit    480
#define raw_byte ((PINC<<4)|(PIND>>4));

#define frame_header0 0xFF
#define frame_header1 0x87

#define row_header0   0xFF
#define row_header1   0x11

volatile uint8_t flag_VSYNC=0;    // flag that indicates VSYNC

volatile uint16_t row_count = 0;
volatile uint8_t buffer[buffer_max];
volatile uint16_t buffer_count_in  = 0;
volatile uint16_t buffer_count_out = 0;

volatile uint8_t f_num = 0;
#define f_limit 1

int main(void)
{
    //uint8_t chk;
    rev_STDIO_set();
    setup();
    // printf("start\n");
    sei();

    while(1)
    {
        if((f_num >= f_limit) && (buffer_count_in == buffer_count_out))
        {
            printf("\nf_limit reach\n");
            while(1);
        }
        /**  sense VSYNC **/
        if((VSYNC&PINB) && (!flag_VSYNC)) {
            flag_VSYNC=1;
            buffer[buffer_count_in]   = frame_header0;
            buffer[buffer_count_in+1] = frame_header1;
            buffer[buffer_count_in+2] = row_header0;
            buffer[buffer_count_in+3] = row_header1;
            buffer[buffer_count_in+4] = row_count;
            buffer_count_in += 5;
            if(buffer_count_in >= buffer_max)
                buffer_count_in -= buffer_max;
            //printf("flag=%d\n",flag_VSYNC);
        }
        //-------------//
        /** UART0 **/
        if(buffer_count_in != buffer_count_out)
        {
            while(!(UCSR0A & (1 << UDRE0)));
            UDR0 = buffer[buffer_count_out];
            buffer_count_out ++;
            if(buffer_count_out >= buffer_max)
                buffer_count_out -= buffer_max;
        }
        //-------------//
        /*
        if(row) {
            for (count=0;count<64;count++){
                 printf("%d\t",data[count][row2]);
            }
            printf("\nrow=%d\n",row2);

            if(row2==10) {while(1); }
            row2++;
        }
        */

        //printf("START %.2X\n",PIND);
    }
}

/** PCLK trigger**/
ISR(INT0_vect)
{
    if(flag_VSYNC)
    {
        if(PIND & HREF)
        {
            buffer[buffer_count_in] = raw_byte;
            buffer_count_in ++;
            if(buffer_count_in >= buffer_max)
                buffer_count_in -= buffer_max;
        }
        else
        {
            row_count ++;
            if(row_count == row_limit)
            {
                f_num ++;
                flag_VSYNC = 0;
                cli();
            }
            else
            {
                buffer[buffer_count_in] = row_header0;
                buffer[buffer_count_in+1] = row_header1;
                buffer[buffer_count_in+2] = row_count;
                buffer_count_in += 3;
                if(buffer_count_in >= buffer_max)
                    buffer_count_in -= buffer_max;
            }
        }
    }
}
