#include <avr/io.h>
#include <stdio.h>

#define TWI_START 0x08
#define TWI_timeout 0x01
#define timeout_count 20
#define TWI_STATUS (TWSR & 0xF8)

void twi_ini (void) {
	TWBR=72;
	TWSR=0<<TWPS1 | 0<<TWPS0;
}

uint8_t start (void) {
    uint8_t count=0;
	TWCR=(1<<TWEN) | (1<<TWSTA) | (1<< TWINT);
	while (!(TWCR&(1<<TWINT)))
    {
        count ++;
        if(count > timeout_count)
            return TWI_timeout;
    }
	if(TWI_STATUS != TWI_START) {
        printf("Exp %.2X ERR %.2X\n", TWI_START, TWI_STATUS);
        return TWI_STATUS;
	}
    return 0;
}
uint8_t write_addr (uint8_t addr,uint8_t ACK) {
    uint8_t count=0;
    TWDR=addr;
	TWCR=1<<TWINT | 1<<TWEN;
	while (!(TWCR&(1<<TWINT)))
    {
        count ++;
        if(count > timeout_count)
            return TWI_timeout;
    }
	if(TWI_STATUS != ACK) {
        printf("Exp %.2X ERR %.2X\n", ACK, TWI_STATUS);
        return TWI_STATUS;
	}
    return 0;
}
uint8_t write_data (uint8_t data,uint8_t ACK) {
    uint8_t count=0;
	TWDR=data;
	TWCR=1<<TWINT | 1<<TWEN;
	while(!(TWCR&(1<<TWINT)))
    {
        count ++;
        if(count > timeout_count)
            return TWI_timeout;
    }
	if(TWI_STATUS != ACK) {
        printf("Exp %.2X ERR %.2X\n", ACK, TWI_STATUS);
        return TWI_STATUS;
	}
    return 0;
}

uint8_t read(uint8_t nACK){
    uint8_t count=0;
    if (nACK){
        TWCR = (1<<TWINT) | (1<<TWEN);
        while (!(TWCR&(1<<TWINT)))
        {
            count ++;
            if(count > timeout_count)
                return TWI_timeout;
        }
        if (TWI_STATUS != 0x58)
          printf("-----read\n");
        return TWDR;
    }
    else{
        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
        while (!(TWCR&(1<<TWINT)))
        {
            count ++;
            if(count > timeout_count)
                return TWI_timeout;
        }
        if (TWI_STATUS != 0x50)
          printf("-----read\n");
        return TWDR;
    }
}
