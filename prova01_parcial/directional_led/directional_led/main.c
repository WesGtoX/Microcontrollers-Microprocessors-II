/* Engenharia da Computação - UNAERP
 * Prova Parcial - Microcontroladores e Microprocessadores II - GL522A
 * 
 * directional_led.c
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
#define LED_09	(0)

int led_status = 1, value = 0, direction = 0, led_nine = 1;

ISR(TIMER1_COMPA_vect) {
	if (led_nine == 1) {					// verifica o estado do LED
		PORTC = PORTC | (1<<LED_09);		// seta para o status do LED para aceso
	}
	
	if (led_nine == 0) {					// verificar o estado do LED
		PORTC = PORTC & ~(1<<LED_09);		// seta para o status do LED para apagado
	}
	
	led_nine = !led_nine;			// seta valor do LED para seu contrario
}


int main(void) {
	DDRB |= (0xFF);					// set output to all PORTB
	DDRC |= (0x01);					// set output to all PORTC
	
	TCCR1B |= (0x0B);				// pre-scaler = 64, clear timer/counter e compare match
	OCR1A = 5;						// define top counter value
	TIMSK1 |= (0x02);				// enable OC1 output to 'overflow interrupt'
	
	int i = 0;
	
	sei();							// habilita interrupcao
	
	while (1) {
		if (led_status == 1) {
			value = i;
			PORTB |= (1 << value);	// PORTB LED NL 1
			_delay_ms(300);			// delay 300 milliseconds
			led_status = 0;			// set LED Status to NL0
		}
		
		if (led_status == 0) {
			value = 8 - (8 - i);
			PORTB &= ~(1 << value);	// PORTB LED NL 0
			led_status = 1;			// set LED Status to NL1
		}
		
		
		if (PINC & (1 << PINC1)) {
			direction = 1;
			PORTC &= ~(1 << PINC1);
		}
			
		
		if (PINC & (1 << PINC2)) {
			direction = 0;
			PORTC &= ~(1 << PINC2);
			
		}
		

		if (direction == 0) {		// define a direcao do LED
			i--;					// para esquerda
		} else {
			i++;					// para direita
		}

		if (i >= 8) {				// controle de LED
			i = 0;
		}
		
		if (i < 0) {				// controle de LED
			i = 8;
		}
	}
}