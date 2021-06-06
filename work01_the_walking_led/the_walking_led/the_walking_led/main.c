/* Engenharia da Computação - UNAERP
 * Trabalho Parcial - Microcontroladores e Microprocessadores II - GL522A
 * 
 * the_walking_led.c
 *
 * 
 * Author : Wesley de Oliveira Mendes - 828.507
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#ifdef F_CPU
#undef F_CPU
#define F_CPU 16000000UL			// CPU frequency
#endif

#define TIMER_CLOCK	2				// define led 9 freq to 1 Hz

#define LED_01	(0)
#define LED_02	(1)
#define LED_03	(2)
#define LED_04	(3)
#define LED_05	(4)
#define LED_06	(5)
#define LED_07	(6)
#define LED_08	(7)

int led_status = 1, value = 0;

int main(void) {
	DDRB |= (0xFF);					// set output to all PORTB
	TCCR1B |= (0x0B);				// pre-scaler = 64, clear timer/counter e compare match
	OCR1A = 5;						// define top counter value
	TIMSK1 |= (0x02);				// enable OC1 output to 'overflow interrupt'
	
	int i = 0;
	
	PORTB = (0xFF);					// PORTB LED NL 1
	_delay_ms(500);					// delay 5 milliseconds
	PORTB = (0x00);					// PORTB LED NL 0
	_delay_ms(500);					// delay 5 milliseconds
	
	while (1) {
		if (led_status == 1) {
			value = i;
			PORTB |= (1 << value);	// PORTB LED NL 1
			_delay_ms(1000);		// delay 10 milliseconds
			led_status = 0;			// set LED Status to NL0
		}
	
		if (led_status == 0) {
			value = 8 - (8 - i);
			PORTB &= ~(1 << value);	// PORTB LED NL 0
			led_status = 1;			// set LED Status to NL1
		}
		
		if (i == 7) {
			i = 0;					// if i == 0 restart process
		} else {
			i++;
		}
		
	}
}