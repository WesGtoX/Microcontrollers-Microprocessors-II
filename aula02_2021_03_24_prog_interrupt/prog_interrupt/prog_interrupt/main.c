/*
 * prog_interrupt.c
 *
 * Created: 24/03/2021 10:14:16
 * Author : Wesley
 */ 

#include <inttypes.h>           // tipos inteiros
#include <avr/io.h>             // E/S padrão
#include <avr/interrupt.h>      // controle das interrrupções

#define F_CPU       16000000UL  // frequencia da CPU em Hz
#define TIMER_CLOCK       2     // frequencia do LED em 1Hz
#define tbi(x,y) x ^= _BV(y)
static volatile uint8_t led;    // o termo 'volatile' eh usado quando a
                                // variavel é acessada em interrupções

ISR(TIMER1_COMPA_vect)          // chamda quando OC1 gerar 'overflow interrupt'
{
    PORTB = led;
    led <<= 1;                  // move para o proximo LED
    if (!led)                   // overflow: começa no bit 0 de novo
    led = 1;
}

int main(void)
{
    DDRD = 0xff;
    PORTD = 0x00;
    DDRB  = 0xff;               // todos os pinos da porta B como OUTPUT
    PORTB = 0x00;               // desliga todos os LEDs
    TCCR1B = _BV(CS10) | _BV(CS11)  | _BV(WGM12);       // prescaler=64, clear timer/counter e compareA match
    OCR1A = ((F_CPU/2/64/TIMER_CLOCK) - 1 );
    // habilita a saida de OC1 (Output Compare 1) para 'overflow interrupt'
    TIMSK1  = _BV(OCIE1A);
    led = 1;                    // estado inicial da variavel LED
    sei();                      // habilita as interrupções
    unsigned int i;
    while(1==1)                 // loop infinito
    {
        for(i=0;i<65535;i++);   // gera um delay
        for(i=0;i<65535;i++);   // gera um delay
        tbi(PORTD,PD0);         // aqui faz o toggle
    }                           // loop infinito
}

