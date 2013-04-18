/******************************************************************************
 * main.c
 * 
 * Autor: Alejandro Weinstein
 * Licencia: New BSD 
 * Descripcion:
 * Lee un byte desde el puerto serial virtual (USB) y lo envia de vuelta
 *
 * Modificaciones (SSepulveda)
 * Senala error al sobrepasar el maximo tamano del buffer
 * Fix write_buffer
 ******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdlib.h>
#include <ctype.h>
#include "serial.h"
#include "blink.h"
#include "main.h"

#define led_on() LED_PORT |= (1 << LED_PIN)
#define led_off() LED_PORT &= ~(1 << LED_PIN)
#define led_complement() LED_PORT ^= (1 << LED_PIN)

/* Prototipo de funciones */
void timer0_init(void);
void write_buffer(char *buffer, uint8_t n);

/* Variables globales */
volatile uint8_t Timer0_Flag;

int main(void)
{
    int16_t data_in;
    uint16_t blink_T, blink_count;
    uint8_t buffer_i;
    char buffer[4];
    int N=sizeof(buffer);
    char error[]="Error!";

    blink_T = 25;
    blink_count = 0;

    buffer_i = 0;
    buffer[0] = '\0';

    timer0_init();
    serial_init();
    sei();

    while(1) {
	serial_task();
	data_in = serial_read();
	if (data_in >= 0) {
	    if (isdigit(data_in)) {
		buffer[buffer_i++] = (uint8_t)data_in;
	    }
	    
	    if (data_in == 0x0A || data_in == 0x0D) {
		if(buffer_i>N || buffer_i<1){
		    write_buffer(error,sizeof(error)-1);
		    buffer[0]='\0';
		    buffer_i=0;
		} else {
		    buffer[buffer_i] = '\0';
		    blink_T = atoi(buffer);
		    write_buffer(buffer,buffer_i);
		    buffer[0] = '\0';
		    buffer_i = 0;
		}
	    }
	}

	if (Timer0_Flag) {
	    blink_count ++;
	    if (blink_count >= blink_T) {
		led_complement();
		blink_count = 0;
	    }
	    Timer0_Flag = FALSE;
	}
    }
    return 0;
}

void write_buffer(char *buffer, uint8_t n)
{
    int i=0;
    while(i<n){
	serial_write(buffer[i++]);
    }
    serial_write(0x0A);
}

void timer0_init(void)
{
    TCCR0B = (1 << CS02) | (1 << CS00); 
    // PS=1024 => 15625 MHz / 6.4e-5 s @ 16 MHz
    // 1ms timeout => 16 ticks
    TCNT0 = 0xFF - 16;
    TIMSK0 |= (1 << TOIE0);
}

ISR(TIMER0_OVF_vect)
{
    TCNT0 = 0xFF - 16;
    Timer0_Flag = TRUE;
}

/*

  - ver tabla de interrupciones en Sec. 9 de la hoja de datos.

*/
