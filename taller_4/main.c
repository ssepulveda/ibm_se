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
#include <avr/interrupt.h>
#include "serial.h"

/* Function prototype*/
void LUFA_Init(void);
void USART_Init(unsigned int baud);
unsigned char USART_Receive(void);
void USART_Transmit(unsigned char data);
void USART_Flush(void);

/* Global variables */
volatile char data[5];

/* Main */
int main(void){
    USART_Init(207);
    sei();
    LUFA_Init();
    
    while(1){
        serial_task();
        //serial_write((unsigned char)USART_Receive());
        getData();
        //decodeData();
    }
    return 0;
}

/* Interruption handler */
ISR(USART1_RX_vect){
    serial_write(0xff);
    UCSR1A=0<<RXC1;
}

/* decode funtions */
void getData(void){
    unsigned char tmp;
    int sync=0;

    // finds sync bit identifying first data package
    tmp=USART_Receive();
    if(tmp&(1<<7)){
        data[0]=tmp;
        sync=1;
    
    // saves the rest of the data packages
    while(sync<5){
        tmp=USART_Receive();
        if(!(tmp&(1<<7))){
            data[sync]=tmp;
            sync++;
        }
    }
    sync=0;
}

/*
void decodeData(void){
    // TEST DATA 0b11000100
    int pulseIntesity=data[0]&3;
    int oxygenDissolved=data[1]&6;
    int pulseSound=data[2]&3;
    int pulseRate=data[3]&6;
    int oxygenSaturation=data[4]&6;
    
    //serial_write((unsigned char)pulseIntesity);
    //serial_write('\n');
    //serial_write((char)oxygenSaturation);
    //serial_write('\r');
}
*/

/* LUFA functions */
void LUFA_Init(void){
    // Enable LUFA
    serial_init();
}

/* USART functions */
void USART_Init(unsigned int baud){
    // Set baud rate (from table 18-12 pp213)
    UBRR1H=(unsigned char)(baud>>8);
    UBRR1L=(unsigned char)baud;
    
    // Set frame format 8 data (UCSZ1) 1 stop bit (USBS1)
    //UCSR1C = (0<<USBS1)|(1<<UCSZ10)|(1<<UCSZ11);
    UCSR1C=(3<<UCSZ10);

    // Parity pp208
    //UCSR1C=(1<<UPM11)|(0<<UPM10);

    // Enable interruption pp206
    //UCSR1B=(1<<RXCIE1);

    // Enable receiver and transmitter
    //UCSR1B = (1<<RXEN1)|(1<<TXEN1);
    UCSR1B=(1<<RXEN1);
}

unsigned char USART_Receive(void){
    // Wait for data to be received
    while(!(UCSR1A & (1<<RXC1)));

    // Get and return received data from buffer
    return UDR1;
}

void USART_Transmit(unsigned char data){
    // Wait for empty transmit buffer
    while(!( UCSR1A & (1<<UDRE1)));

    // Put data into buffer, sends the data
    UDR1 = data;
}

void USART_Flush(void){
    unsigned char dummy;
    while(UCSR1A & (1<<RXC1)){
        dummy = UDR1;
    }
}