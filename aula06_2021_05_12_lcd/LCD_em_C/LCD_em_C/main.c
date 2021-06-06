/*
 * LCD_em_C.c
 *
 * 
 * Autor : Carlos Formigoni
 NOTA : Por convenção os nomes das funções foram colocadas em ingles
         para ficar mais facil a compatibilidade com o datasheet do LCD
		 usado.
 */ 

/****************************************************************************
    LCD_em_C.c  - Usa um HD44780U conectado a um ATMEGA328p               
 */
/****************************************************************************
      Summario:    Interface de 4 bits para o LCD.  
                  Os pinos do LCD s�o conectados na portaD do ATMEGA.    
 */
/******************************* Observação ******************************
Diagrama de conexão: 
                 -----------                   ----------
                | ATmega328 |                 |   LCD    |
                |           |                 |          |
                |     13-PD7|---------------->|D7        |
                |     12-PD6|---------------->|D6        |
                |     11-PD5|---------------->|D5        |
                |      6-PD4|---------------->|D4        |
                |           |                 |D3        |
                |           |                 |D2        |
                |           |                 |D1        |
                |           |                 |D0        |
                |           |                 |          |
                |     15-PB1|---------------->|E         |
                |           |         GND --->|RW        |
                |     14-PB0|---------------->|RS        |
                 -----------                   ----------
 
  **************************************************************************/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

// LCD interface (de acordo com o diagrama anterior)
//   pino RW deve ligado ao GND
#define lcd_D7_port     PORTD                   // lcd D7 
#define lcd_D7_bit      PORTD7
#define lcd_D7_ddr      DDRD

#define lcd_D6_port     PORTD                   // lcd D6 
#define lcd_D6_bit      PORTD6
#define lcd_D6_ddr      DDRD

#define lcd_D5_port     PORTD                   // lcd D5 
#define lcd_D5_bit      PORTD5
#define lcd_D5_ddr      DDRD

#define lcd_D4_port     PORTD                   // lcd D4 
#define lcd_D4_bit      PORTD4
#define lcd_D4_ddr      DDRD

#define lcd_E_port      PORTB                   // lcd Enable 
#define lcd_E_bit       PORTB1
#define lcd_E_ddr       DDRB

#define lcd_RS_port     PORTB                   // lcd Register Select
#define lcd_RS_bit      PORTB0
#define lcd_RS_ddr      DDRB

// LCD endereço da RAM interna do LCD
#define lcd_LineOne     0x00                    // endereço de inicio da linha 1
#define lcd_LineTwo     0x40                    // endereço de inicio da linha 2
// LCD - instruções
#define lcd_Clear           0b00000001          // troque todos os caracteres por 'space'
#define lcd_Home            0b00000010          // cursor na primeira posição da 1a linha
#define lcd_EntryMode       0b00000110          // move o cursor da esquerda para direita (R/W)
#define lcd_DisplayOff      0b00001000          // display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off,sem piscar caracter
#define lcd_FunctionReset   0b00110000          // reset no LCD
#define lcd_FunctionSet4bit 0b00101000          // 4-bit data, 2-linhas display, 5 x 7 na fonte
#define lcd_SetCursor       0b10000000          // acerta a posição do cursor

// Programa - Identificação
uint8_t programa_autor[]   = "-=-WESLEY-=-";
uint8_t programa_versao[]  = "LCD-AVR-7 (gcc)";
uint8_t programa_data[]     = "Maio, 2021";

// Funções usadas
void lcd_write_4(uint8_t);
void lcd_write_instruction_4d(uint8_t);
void lcd_write_character_4d(uint8_t);
void lcd_write_string_4d(uint8_t *);
void lcd_init_4d(void);



/******************************* Código principal*************************/
int main(void) {
// configura o ATMEGA com os  pinos p/ as linhas de dados
    lcd_D7_ddr |= (1<<lcd_D7_bit);                  // 4 linhas - output
    lcd_D6_ddr |= (1<<lcd_D6_bit);
    lcd_D5_ddr |= (1<<lcd_D5_bit);
    lcd_D4_ddr |= (1<<lcd_D4_bit);

// configura o ATMEGA para as linhas de controle
    lcd_E_ddr |= (1<<lcd_E_bit);                    // E  - output
    lcd_RS_ddr |= (1<<lcd_RS_bit);                  // RS - output

// initializa o LCD 
    lcd_init_4d();                                  // initializa para 4-bit de interface

// display na primeira linha
    lcd_write_string_4d(programa_autor);

// ajusta cursor na segunda linha
    lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
    _delay_us(80);                                  

// display na segunda linha
    lcd_write_string_4d(programa_versao);

//  loop infinito
    while(1);
    return 0;
}
/******************************* fim do código principal ******************/

/*============================== Funções de 4 bits do LCD ===============*/

void lcd_init_4d(void) {
// Power-up delay
    _delay_ms(100);                                 // inicial de 40 mSeg de delay

// IMPORTANTE - Neste ponto o LCD esta no modo de 8 bits e espera receber 8 bits de dados.        
//   para cada vez que o pino E é pulsado.                                                   
//
// Como o LCD esta ligado com apenas os 4 bits superiores nas linhas de dados ao ATMEGA              
//   as 4 linhas de dados do LCD ficam livres.Assim quando o sinal E for pulsado o LCD lá 
//   os 4 bits da parte superior e os 4 bits da parte inferior ficarão em 1. 
//   

// Set up para o sinal RS e E para a sub rotina 'lcd_write_4'.
    lcd_RS_port &= ~(1<<lcd_RS_bit);                // seleciona o registrador de instrução (RS low)
    lcd_E_port &= ~(1<<lcd_E_bit);                  // faz com que E esteja em 0

// Reset no LCD controller
    lcd_write_4(lcd_FunctionReset);                 // primeira parte da sequencia do reset
    _delay_ms(10);                                  // 4.1 mS delay no minimo

    lcd_write_4(lcd_FunctionReset);                 // segunda parte do reset
    _delay_us(200);                                 // 100uS delay no minimo

    lcd_write_4(lcd_FunctionReset);                 // terceira parte da sequencia do reset
    _delay_us(200);                                 // tempo final

    lcd_write_4(lcd_FunctionSet4bit);               // ajusta para o modo  4-bits 
    _delay_us(80);                                  // 40uS delay (minimo)

// Function Set instruction
    lcd_write_instruction_4d(lcd_FunctionSet4bit);   // programa o modo, linhas e fonte
    _delay_us(80);                                  // 40uS delay (minimo)

// As proximas 3 instruções est�o no datasheet do LCD para a parte de inicialização. 


// Display On/Off Control 
    lcd_write_instruction_4d(lcd_DisplayOff);        //  display OFF
    _delay_us(80);                                  // 40uS delay (min)

// Clear Display n
    lcd_write_instruction_4d(lcd_Clear);             // display RAM
    _delay_ms(4);                                   // 1.64 mS delay (min)

// ; entra no modo de instrução
    lcd_write_instruction_4d(lcd_EntryMode);         // modo de entrada
    _delay_us(80);                                  // 40uS delay (min)

 
// Display On/Off 
    lcd_write_instruction_4d(lcd_DisplayOn);         // display ON
    _delay_us(80);                                  // 40uS delay (min)
}

/*...........................................................................
  Nome:     lcd_write_string_4d
*/                                                        
void lcd_write_string_4d(uint8_t theString[]) {
    volatile int i = 0;                             // contador de caracter*/
    while (theString[i] != 0)
    {
        lcd_write_character_4d(theString[i]);
        i++;
        _delay_us(80);                              // 40 uS delay (min)
    }
}

/*...........................................................................
  Nome:     lcd_write_character_4d
*/

void lcd_write_character_4d(uint8_t theData) {
    lcd_RS_port |= (1<<lcd_RS_bit);                 // seleciona o Data Register (RS em alto)
    lcd_E_port &= ~(1<<lcd_E_bit);                  // garante que E é zero
    lcd_write_4(theData);                           // envia os  4-bits superiores do dado
	lcd_write_4(theData << 4);                      // envia os 4 bits inferiores do dado
}

/*...........................................................................
  Nome:     lcd_write_instruction_4d
 */
void lcd_write_instruction_4d(uint8_t theInstruction) {
    lcd_RS_port &= ~(1<<lcd_RS_bit);                // seleciona o registrador de instrução (RS zero)
    lcd_E_port &= ~(1<<lcd_E_bit);                  // garante que E é zero
    lcd_write_4(theInstruction);                    // envia os  4-bits superiores do dado
    lcd_write_4(theInstruction << 4);               // envia os 4 bits inferiores do dado
}


/*...........................................................................
  Nome:     lcd_write_4
*/
void lcd_write_4(uint8_t theByte) {
    lcd_D7_port &= ~(1<<lcd_D7_bit);                        // assume que o dado é '0'
    if (theByte & 1<<7) lcd_D7_port |= (1<<lcd_D7_bit);     // faz dado = '1' se necessario

    lcd_D6_port &= ~(1<<lcd_D6_bit);                        // repete para cada bit
    if (theByte & 1<<6) lcd_D6_port |= (1<<lcd_D6_bit);

    lcd_D5_port &= ~(1<<lcd_D5_bit);
    if (theByte & 1<<5) lcd_D5_port |= (1<<lcd_D5_bit);

    lcd_D4_port &= ~(1<<lcd_D4_bit);
    if (theByte & 1<<4) lcd_D4_port |= (1<<lcd_D4_bit);

// escreve o dado
                                                    // tempo de set up do endereço (40 nS)
    lcd_E_port |= (1<<lcd_E_bit);                   // Enable em alto (E = 1)
    _delay_us(1);                                   // tempo de set up e tempo do pulso E (230 nS)
    lcd_E_port &= ~(1<<lcd_E_bit);                  // Enable em baixa (E = 0)
    _delay_us(1);                                   // tempo de espera do dado (10 nS) e ciclo de ENABLE (500 nS)
}
