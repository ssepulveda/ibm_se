/******************************************************************************
* Taller 3 Sistemas Embebidos
* Departamento Ingenieria Biomedica, Universidad de Valparaiso
*
* Autor: Sebastián Sepúlveda
* Licencia: New BSD
* Descripcion:
* Comunicación serial por medio de LUFA utilizando Arduino Leonardo para
* obtener datos desde el conversor analogo digital A0 (ADC7)
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

/** Guarda la última lectura del ADC
 * @details Guarda los datos obtenidos desde la última lectura del ADC7
 */
volatile char sensorData[5];

/** Función principal de la aplicación
 * @details Se ejecuta al inicial el procesamiento del microcontrolador
 *
 * Inicializa los puertos, protocolos, registros e interrupciones mediante
 * la función @ref init
 *
 * Posteriormente, inicia un loop para realizar una medición del ADC7 y
 * enviar los resultados por el puerto serial
 */
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

/** Inicializa el ADC7, habilita las interrupciones e inicializa LUFA
 * @details Configura y habilita el ADC7 para capturar datos a 125 KHz, 10 bits
 * de resolución y habilita interrupciones al existir un nuevo dato.
 *
 * Posteriormente, inicializa la librería LUFA para establecer
 * comunicación serial 8N1 con el computador.
 */
void init(void){
    /* configuracion del ADC */
    //configurar el puerto ADC7=A0
    ADMUX|=(1<<MUX0 | 1<<MUX1 | 1<<MUX2);
    // confiurar FS del ADC @125 KHz
    ADCSRA=(1<<ADEN | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);
    // configurar resolucion en 8 bits
    //ADMUX|=1<<ADLAR;
    // configurar referencia para conversion
    ADMUX|=1<<REFS0;
    // habilitar la interrupcion del ADC
    ADCSRA|=1<<ADIE;
    // habilitar ADC
    ADCSRA|=1<<ADEN;
    // iniciar la conversion ADC
    ADCSRA|=1<<ADSC;

    /* Habilitar interrupciones globales */
    sei();
    
    /* habilitar el puerto serial */
    serial_init();
}

/** Envía el resultado del ADC7 al computador
 * @details Envía un arreglo de char por el puerto serial utilizando la
 * librería LUFA.
 * @param buffer arreglo de char a enviar
 */
void write_serial(char *buffer)
{
    int i=0;
    while(buffer[i]!='\0'){
	serial_write(buffer[i++]);
    }
    serial_write(0x0A);
}

/** Habilita el ADC7 para realizar una nueva lectura
 * @details Permite una nueva lectura para que @ref ISR pueda actualizar
 * los datos
 */
void read_data(void)
{
    // habilitar una nueva lectura
    ADCSRA|=1<<ADSC;
}

/** Maneja las interrupciones
 * @params ADC_vect interrupción por ADC
 * @details Actualiza el valor de la última medición obtenida desde el ADC7
 * actualizando el valor directamente a @ref sensorData como char
 */
ISR(ADC_vect)
{
    // Convertir resultado en string
    uint8_t low=ADCL;
    itoa((ADCH<<8|low),sensorData,10);
}
