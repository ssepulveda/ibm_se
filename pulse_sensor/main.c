/******************************************************************************
 * Main.c
 * 
 * Autor: Sebastian Sepulveda
 * Licencia: New BSD 
 * Descripcion:
 * Arduino Leonardo + pulse sensor amp
 *
 ******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <ctype.h>
#include <stdlib.h>
#include "serial.h"

// Prototipo de funciones
void init(void);
void write_serial(char *buffer);
void read_data(void);

// Variables globales
volatile char sensorData[4];

int main(void)
{
    init();
    
    while(1){
	serial_task();
	write_serial(sensorData);
	// obtener nuevas mediciones
	read_data();
    }
    return 0;
}

void init(void){
    //*** configuracion del ADC
    //configurar el puerto ADC7=A0
    ADMUX|=(1<<MUX0 | 1<<MUX1 | 1<<MUX2);
    // confiurar FS del ADC
    ADCSRA|=1<<ADPS2;
    // configurar bits del ADC 8 bits
    ADMUX|=1<<ADLAR;
    // configurar referencia para conversion
    ADMUX|=1<<REFS0;
    // free-running mode
    //ADCSRA|=1<<ADFR;
    // habilitar la interrupcion del ADC
    ADCSRA|=1<<ADIE;
    // habilitar ADC
    ADCSRA|=1<<ADEN;
    // iniciar la conversion ADC
    ADCSRA|=1<<ADSC;
    /* fin de configuracion del ADC */

    //*** Habilitar interrupciones globales
    sei();
    //*** habilitar el puerto serial
    serial_init();
}

void write_serial(char *buffer)
{
    int i=0;
    int N=sizeof(buffer);
    while(i<=N){
	serial_write(buffer[i++]);
    }
    serial_write(0x0A);
}

void read_data(void)
{
    // habilitar una nueva lectura
    ADCSRA|=1<<ADSC;
}

// Interruption handler
ISR(ADC_vect)
{
    // Convertir resultado en string
    itoa(ADCH,sensorData,10);
}
