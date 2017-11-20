#include ".\LIB\rev_STDIO.h"
#include <stdio.h>
#include <avr/io.h>
#include "lib_OV7670.c"
#include <util/delay.h>
#include <avr/interrupt.h>

char vsyc=0b00000001;
char flag87=0;
char href=0b00001000;
int row=0;
int count=0;
int data[64][10]={0};
int row2=0;
int main(void)
{
    //uint8_t chk;
    rev_STDIO_set();
    setup();
    printf("start\n");
    sei();

    while(1)
    {
        if(vsyc&PINB) {
            flag87=1;
            //_delay_ms(1000);
            printf("flag=%d\n",flag87);
        }
        if(row) {
            for (count=0;count<64;count++){
                 printf("%d\t",data[count][row2]);
            }
            printf("\nrow=%d\n",row2);

            if(row2==10) {while(1); }
            row2++;
        }


        //printf("START %.2X\n",PIND);
    }
}


ISR(INT0_vect) {

    if (flag87==1) {

        if (href&PIND) {
                data[count][row]=raw_byte;
                count++;
            }

            if(count==64) {row++;count=0;}
            if(row==10)   {flag87=0;cli();}
        }

}
