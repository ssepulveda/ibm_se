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
volatile char data[5];
volatile char bin2hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/* Main */
int main(void){
    USART_Init(207);
    LUFA_Init();
    
    while(1){
        serial_task();
        getData();
        sendData();
    }
    return 0;
}

/* decode funtions */
void getData(void){
    unsigned char tmp;
    int sync=0;

    // finds sync bit identifying first data package
    while(sync==0){
        tmp=USART_Receive();
        if(tmp&(1<<7)){
            data[0]=tmp;
            sync=1;
        }
    }
    
    for(int i=1;i<=4;i++){
        tmp=USART_Receive();
        if(!(tmp&(1<<7)) && tmp!=data[i-1] && sync==1){
            data[i]=tmp;
        }else{
            i--;
        }
    }
}

void sendData(void){
    // numeric data
    uint8_t pulseIntesity=data[0]&4;
    uint8_t oxygenDissolved=data[1]&7;
    uint8_t pulseSound=data[2]&4;
    uint8_t pulseRate=data[3]&7;
    uint8_t oxygenSaturation=data[4]&7;

    // status data
    uint8_t searchTimeTooLong='E';
    uint8_t oxygenSaturationDecrease='E';
    uint8_t probeError='E';
    uint8_t searchPulse='E';

    if(!(data[0]&(1<<4))){
        searchTimeTooLong='K';
    }
    if(!(data[0]&(1<<5))){
        oxygenSaturationDecrease='K';
    }
    if(!(data[2]&(1<<4))){
        probeError='K';
    }
    if(!(data[2]&(1<<5))){
        searchPulse='K';
    }

    // print data to LUFA
    serial_writeBin(pulseIntesity);
    serial_write(',');
    serial_writeBin(oxygenDissolved);
    serial_write(',');
    serial_writeBin(pulseSound);
    serial_write(',');
    serial_writeBin(pulseRate);
    serial_write(',');
    serial_writeBin(oxygenSaturation);
    serial_write(',');
    serial_write(searchTimeTooLong);
    serial_write(',');
    serial_write(oxygenSaturationDecrease);
    serial_write(',');
    serial_write(probeError);
    serial_write(',');
    serial_write(searchPulse);
    serial_write('\n');
    serial_write('\r');
}

void serial_writeBin(uint8_t bin){
    serial_write(bin2hex[(bin&0xF0)>>4]);
    serial_write(bin2hex[bin&0x0F]);
}

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