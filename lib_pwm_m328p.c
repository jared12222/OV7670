#include <avr/io.h>

void pwm_ini (void) {
    // set OCR2A pwm at CTC 8MHz
    DDRB |= 1 << 3;
	TCCR2A = _BV(COM2A0) + _BV(WGM21);
    TCCR2B = _BV(CS20);
    OCR2A = 0;
}
