#include <avr/io.h>

void pwm_ini (void) {
    DDRB |= 1 << 3;
	TCCR2A = _BV(COM2A0) + _BV(WGM21);
    TCCR2B = _BV(CS20);
    OCR2A = 0;
}
