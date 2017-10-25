#include <avr/io.h>

#define TWI_START 0x08


void twi_ini (void) {
	TWBR=72;
	TWSR=0<<TWPS1 | 0<<TWPS0;
}

void start (void) {
	TWCR=1<<TWEN | 1<<TWSTA | 1<< TWINT;
	while (!(TWCR&(1<<TWINT)));
	if(TWSR&0xF8 != TWI_START) {
		printf("-----start\n");
		while(1);
	}
}
void write_addr (uint8_t addr,uint8_t ACK) {
	TWDR=addr;
	TWCR=1<<TWINT | 1<<TWEN;
	while(TWCR&(1<<TWINT) == 0);
	if(TWSR&0xF8 != ACK) {
		printf("-----write\n");
		while(1);
	}
}
void write_data (uint8_t data,uint8_t ACK) {
	TWDR=data;
	TWCR=1<<TWINT | 1<<TWEN;
	while(!(TWCR&(1<<TWINT)));
	if(TWSR&0xF8 != ACK) {
		printf("-----write data\n");
		while(1);
	}
}

uint8_t read(uint8_t nACK){
  if (nACK){
    TWCR = (1<<TWINT) | (1<<TWEN);
    while ((TWCR & 1<<TWINT) == 0); /* wait for transmission */
    if ((TWSR & 0xF8) != 0x58)
      printf("-----read\n");
    return TWDR;
  }
  else{
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    while ((TWCR & (1<<TWINT)) == 0); /* wait for transmission */
    if ((TWSR & 0xF8) != 0x50)
      printf("-----read\n");
    return TWDR;
  }
}
