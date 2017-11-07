#include <stdio.h>
#include <avr/io.h>

#define F_SCL 320000                // Frequncy of SCL
#define timeout 20

/** status of twi **/
#define TIMEOUT_warning  0x01

#define START            0x08
#define RE_START         0x10
#define SLA_W_Trans_ACK  0x18
#define SLA_W_Trans_NACK 0x20
#define SLA_R_Trans_ACK  0x40
#define SLA_R_Trans_NACK 0x48
#define Data_Trans_ACK   0x28
#define Data_Trans_NACK  0x30
#define Data_Receiv_ACK  0x50
#define Data_Receiv_NACK 0x58
#define Arb_SLAW         0x38


#define TWI_STATUS (TWSR & 0xF8)    //TWI Status flag

#define TWI_START    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN)

#define TWI_Trans(data)                 \
    TWDR = data;                        \
    TWCR = (1 << TWINT)|(1 << TWEN);

#define TWI_start_operation TWCR = (1 << TWINT) | (1 << TWEN)

#define TWI_STOP    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO)

#define TWI_ERR(Exp,chk)                        \
    if(chk)                                     \
        printf("Expect %X ERR %X\n",Exp,chk);

/**
    Initial TWI by Frequency F_SCL, no TWI_int
**/
void TWI_ini(void)
{
    TWBR = (F_CPU/F_SCL-16)/2;      // by prescaler 1
    TWCR = (1 << TWEN);             // Enable TWI
    TWSR |= 0;                      // Prescaler = 1 (4^n, n=0,1,2,3)
}

uint8_t TWI_wait(uint8_t expect)
{
    uint8_t count;
    while(!(TWCR & (1<<TWINT)))
    {
        count ++;
        if(count >= timeout)
            return TIMEOUT_warning;
    }
    if(TWI_STATUS != expect)
        return TWI_STATUS;
    return 0;
}



uint8_t SCCB_write(uint8_t id,uint8_t reg, uint8_t *data,uint8_t n)
{
    uint8_t chk,ind;
    TWI_START;
    chk = TWI_wait(START);
    TWI_ERR(START,chk);
    TWI_Trans((id));
    chk = TWI_wait(SLA_W_Trans_ACK);
    TWI_ERR(SLA_W_Trans_ACK,chk);
    TWI_Trans((reg << 1));
    chk = TWI_wait(SLA_W_Trans_ACK);
    TWI_ERR(SLA_W_Trans_ACK,chk);
    for(ind = 0;ind < n; n++)
    {
        TWI_Trans(*(data+n));
        chk = TWI_wait(Data_Trans_ACK);
        TWI_ERR(Data_Trans_ACK,chk);
    }
    TWI_STOP;
}
