/*
 * interrupt_led.c
 *
 * Timer1 modo de interrupção
 * LED em OC1A e GND
 *
 * Created: 14/04/2021 09:06:05
 * Author : Wesley
 */ 

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

enum {UP, DOWN};
	
ISR(TIMER1_OVF_vect) {
	static uint16_t pwm;
	static uint8_t direcao;
	
	switch (direcao) {
		case UP:
			if (++pwm == 1023) {
				direcao == DOWN;
			}
			break;
		
		case DOWN:
			if (--pwm == 0) {
				direcao == DOWN;
			}
			break;
	}
	
	OCR1A == pwm;
}

 void ioinit(void) {
	 /*timer 1 tem 10 bits PWM (8-bits PWM em alguns ATtinys)*/
	 TCCR1A = ((1 << COM1A1) | (1 << COM1A0));
	 
	 /*start timer1*/
	 TCCR1B = ((1 << CS10) | (1 << WGM13));
	 /*
	 * 
	 /*
	 OCR1A = 0;
	 ICR1 = 0xFF;
	 
	 /* Enable no OC1 para ser saída*/
	 DDRC = _BC(OCR1A);
	 
	 /*Enable timer 1 para overflow interrupt*/
	 TIMSK1 = _BV(TOIE1);
	 sei();
	 
 }

int main(void) {
	ioinit();
	/* loop */
	for (;;)
		OCR1A = 0xA;
	return(0);
}

