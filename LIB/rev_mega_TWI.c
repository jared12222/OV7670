#include <stdio.h>
#include <avr/io.h>

#define F_SCL 320000                // Frequncy of SCL

/** status of twi **/
#define START            0x08
#define RE_START         0x10
#define SLA_W_Trans_ACK  0x18
#define SLA_W_Trans_NACK 0x20
#define SLA_R_Trans_ACK  0x40
#define SLA_R_Trans_NACK 0x48
#define Data_Trans_ACK   0x28
#define Data_Trans_NACK  0x30
#define Data_Receiv_ACK  0x50
#define Data_receiv_NACK 0x58
#define Arb_SLAW         0x38


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

/**
    TWI START condition
**/
uint8_t TWI_START(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while(!(TWCR & (1<<TWINT)));
}
