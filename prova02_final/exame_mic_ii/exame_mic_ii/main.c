/* Engenharia da Computação - UNAERP
 * Prova Final - Microcontroladores e Microprocessadores II - GL522A
 * 
 * exame_mic_ii.c
 *
 * 
 * Author : Wesley de Oliveira Mendes - 828.507
 */ 

#include <avr/io.h>

#define F_CPU 4000000

#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>


// funcao para controlar qual led sera aceso
void led_on(int led) {
	PORTB |= (1<<led);
}


int main(void) {
	// registradores de controle de fluxo
	DDRB = 0xFF;
	DDRC = 0x00;
	
	// selecionar o conversor analogo digital
	ADMUX |=(1<<REFS0);
	// ativar conversor analogo digital
	ADCSRA |=(1<<ADEN)|(1<<ADATE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	
	// varial para controle de tensao
	float voltage = 0;
	
	while(1) {
		// delay para conversao
		_delay_ms(100);
		
		// habilitar inicio de uma nova conversao
		ADCSRA |= (1<<ADSC);
		
		// atribuindo a tensao a variavel
		voltage = (float)(5 * ADC) / 1023;
		
		// verificacao da tensao entre 0 e 0.625
		if (voltage > 0 && voltage <= 0.625) {
			// apagar o led
			PORTB = 0x00;
			// executar funcao para acender o led 1
			led_on(0);
		}
		
		// verificacao da tensao entre 0.625 e 1.25
		if (voltage > 0.625 && voltage <= 1.25) {
			// apagar o led
			PORTB = 0x00;
			// executar funcao para acender o led 2
			led_on(1);
		}
		
		// verificacao da tensao entre 1.25 e 1.875
		if (voltage > 1.25 && voltage <= 1.875) {
			// apagar o led
			PORTB = 0x00;
			// executar funcao para acender o led 3
			led_on(2);
		}
		
		// verificacao da tensao entre 1.875 e 2.5
		if (voltage > 1.875 && voltage <= 2.5) {
			// apagar o led
			PORTB = 0x00;
			// executar funcao para acender o led 4
			led_on(3);
		}

		// verificacao da tensao entre 2.5 e 3.125
		if (voltage > 2.5 && voltage <= 3.125) {
			// apagar o led
			PORTB = 0x00;
			// executar funcao para acender o led 5
			led_on(4);
		}

		// verificacao da tensao entre 3.125 e 3.75
		if (voltage > 3.125 && voltage <= 3.75) {
			// apagar o led
			PORTB = 0x00;
			// executar funcao para acender o led 6
			led_on(5);
		}
		
		// verificacao da tensao entre 3.75 e 4.375
		if (voltage > 3.75 && voltage <= 4.375) {
			// apagar o led
			PORTB = 0x00;
			// executar funcao para acender o led 7
			led_on(6);
		}
		
		// verificacao da tensao entre 4.375 e 5
		if (voltage >  4.375 && voltage <= 5) {
			// apagar o led
			PORTB = 0x00;
			// executar funcao para acender o led 8
			led_on(7);
		}
	}
}
