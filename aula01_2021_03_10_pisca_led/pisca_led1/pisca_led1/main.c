/*
 * pisca_led1.c
 *
 * Created: 24/02/2021
 * Author : formigoni
 */ 

#include <avr/io.h>
#include <util/delay.h>

#ifdef F_CPU
#undef F_CPU
#define F_CPU 16000000UL		// freq da CPU
#endif

#define LED (0)


int main() {
	DDRB |= (1<<LED);			// Ajusta pino LED como output
	while (1) {
		PORTB |= (1<<LED);		// NIVEL LOGICO 1
		_delay_ms(3000);		// delay
		PORTB &= ~(1<<LED);		// NIVEL LOGICO 0
		_delay_ms(3000);		// delay
	}
}

