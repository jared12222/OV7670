#include <stdio.h>
#include <avr/io.h>

#define F_SCL 320000                // Frequncy of SCL

#define TWI_STATUS (TWSR & 0xF8)    //TWI Status flag

/**
    Initial TWI by Frequency F_SCL, no TWI_int
**/
void TWI_ini(void)
{
    TWBR = (F_CPU/F_SCL-16)/2;      // by prescaler 1
    TWCR = (1 << TWEN);             // Enable TWI
    TWSR |= 0;                      // Prescaler = 1 (4^n, n=0,1,2,3)
}

void TWI_START(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while(!(TWCR & (1<<TWINT)));
}
