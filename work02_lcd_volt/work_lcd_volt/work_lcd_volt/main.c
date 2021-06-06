/* Engenharia da Computação - UNAERP
 * Trabalho Parcial - Microcontroladores e Microprocessadores II - GL522A
 *
 * work_lcd_volt.c
 *
 * Author : Wesley Mendes
 */ 

#include <avr/io.h>

#define F_CPU 4000000

#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define E  5
#define RS 6

char content[15] = "WELCOME!!!\0";

ISR(INT0_vect) {
	strcpy(content, "INPUT ADC(0)\0");
	ADMUX &= 0x00;
	ADMUX |= 0x40;
	ADCSRA |=(1<<ADSC);
}

ISR(INT1_vect) {
	strcpy(content,"INPUT ADC(1)\0");
	ADMUX &= 0x00;
	ADMUX |= 0x41;
	ADCSRA |=(1<<ADSC);
}

void send_command(unsigned char command) {
	PORTB = command;
	PORTD &= ~ (1<<RS);
	PORTD |= 1<<E;
	_delay_ms(2);
	PORTD &= ~1<<E;
	PORTB = 0;
}

void send_text(char *character_string) {
	while(*character_string > 0) {
		PORTB = *character_string++;
		PORTD |= 1<<RS;
		PORTD |= 1<<E;
		_delay_ms(2);
		PORTD &= ~1<<E;
		PORTB = 0;
	}
}

int main(void) {
	EICRA = 0x00;
	EIMSK = 0x03;
	
	sei();
	DDRB = 0xFF;
	DDRC = 0x00;
	DDRD = 0x60;
	_delay_ms(50);
	
	ADMUX |=(1<<REFS0);
	ADCSRA |=(1<<ADEN)|(1<<ADATE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	
	float voltage = 0;
	char show_voltage [7];
	
	// Clear Screen 0x01 = 00000001
	send_command(0x01);
	_delay_ms(50);
	send_command(0x38);
	_delay_ms(50);
	send_command(0b00001111);
	_delay_ms(50);
	
	while(1) {
		voltage = strcmp(content, "N/I\0") != 0 ? (float)(5 * ADC) / 1023 : 0;
		send_text (content);
		send_command(0x80 + 0x40 + 0);
		
		send_text ("Volt= ");
		send_command(0x80 + 0x40 + 8);
		
		dtostrf(voltage, 5, 4, show_voltage);
		
		send_text(show_voltage);
		send_text(" V");
		send_command(0x80 + 0);
	}
}
