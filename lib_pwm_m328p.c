#include <avr/io.h>

void pwm_ini (void) {
	TCCR2A=1<<COM2A0 | 1<<WGM21 | 1<<WGM20;
	TCCR2B=1<<WGM22  | 1<<CS20;
	OCR2A=0;
	DDRC &=~15;
	DDRD &=~252;
	_delay_ms(100);
}
