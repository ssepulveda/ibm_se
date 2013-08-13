/******************************************************************************
 * Main.c
 * 
 * Autor: Sebastian Sepulveda
 * Licencia: New BSD 
 * Descripcion:
 * Arduino Leonardo + USART + LUFA
 * Sources: pp 196 datasheet at32u4
 *
 ******************************************************************************/
#include <avr/io.h>
#include "serial.h"

/* Function prototype*/
void LUFA_Init(void);
void USART_Init(unsigned int baud);
unsigned char USART_Receive(void);
void USART_Transmit(unsigned char data);
void USART_Flush(void);

/* Global variables */

/* Main */
int main(void){
    USART_Init(207);
    LUFA_Init();
    
    while(1){
        serial_task();
        //serial_write(USART_Receive());
        USART_Receive();
    }
    return 0;
}

/* Functions */
void LUFA_Init(void){
    // Enable LUFA
    serial_init();
}

void USART_Init(unsigned int baud){
    // Set baud rate (from table 18-12 pp213)
    UBRR1H=(unsigned char)(baud>>8);
    UBRR1L=(unsigned char)baud;
    
    // Set frame format 8 data (UCSZ1) 1 stop bit (USBS1)
    //UCSR1C = (0<<USBS1)|(1<<UCSZ10)|(1<<UCSZ11);
    UCSR1C = (3<<UCSZ10);

    // Parity pp208
    //UCSR1C=(1<<UPM11)|(0<<UPM10);

    // Enable receiver and transmitter
    //UCSR1B = (1<<RXEN1)|(1<<TXEN1);
    UCSR1B = (1<<RXEN1);
}

unsigned char USART_Receive(void){
    /* Wait for data to be received */
    while(!(UCSR1A & (1<<RXC1)));

    //if(UDR1==0b11000100){
    if(UDR1&(1<<7)){
        serial_write('!');
    }else{
        serial_write('x');
    }

    /* Get and return received data from buffer */
    return UDR1;
}

void USART_Transmit(unsigned char data){
    /* Wait for empty transmit buffer */
    while(!( UCSR1A & (1<<UDRE1)));

    /* Put data into buffer, sends the data */
    UDR1 = data;
}

void USART_Flush(void){
    unsigned char dummy;
    while(UCSR1A & (1<<RXC1)){
        dummy = UDR1;
    }
}