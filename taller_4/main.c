/******************************************************************************
* Taller 4 Sistemas Embebidos
* Departamento Ingenieria Biomedica, Universidad de Valparaiso
*
* Autor: Sebastián Sepúlveda
* Licencia: New BSD
* Descripcion:
* Comunicación serial por medio de LUFA y USART utilizando Arduino Leonardo
* como decodificador entre el módulo SPO2 SP100 (PN: MO-SP100) y un computador
*
******************************************************************************/

#include <avr/io.h>
#include "serial.h"

// Function protoype
void LUFA_Init(void);
void USART_Init(unsigned int baud);
unsigned char USART_Receive(void);
void getData(void);
void sendData(void);
void serial_writeBin(uint8_t bin);

/** Bytes obtenidos desde el oxímetro de pulso
 * @details Guarda los datos obtenidos desde el oxímetro de pulso tras la 
 * última adquisición mediante @ref getData
 */
volatile char data[5];
/** Arreglo de conversión de binario a Hexadecimal
 * @details El arreglo permite la conversión de datos binarios a su
 * representación hexadecimal, listando los valores que adquiere para cada
 * valor. Utilizando por la función @ref serial_writeBin.
 */
volatile char bin2hex[16]={'0','1','2','3','4','5','6','7',
                           '8','9','A','B','C','D','E','F'};

/** Funcion principal de la aplicación
 * @details Se ejecuta al inicial el procesamiento del microcontrolador
 * 
 * Inicializa los puertos y protocolos a usar
 * mediante @ref USART_Init para comunicación entre el oxímetro de pulso
 * con el microcontrolador mediante una conexión serial 8N1 a 4800 bps y 
 * @LUFA_Init para la comunicación entre el microcontrolador y un computador, 
 * mediante una conexión serial 8N1 a 115200 bps.
 * 
 * Posteriormente, solicita datos al oxímetro de pulso mediante @ref getData
 * y los envía al computador decodificados mediante @sendData, repitiendo el
 * proceso hasta que se desactive el microcontrolador.
 */
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

/** Adquiere datos desde el oxímetro de pulso
 * @details Según la hoja de información del oxímetro de pulso,
 * los datos se transmiten en paquetes de datos de 5 bytes.
 *
 * La función espera a que el oxímetro de pulso envíe un SYNC bit, detectando
 * el inicio de la transmición del paquete de datos. Comprueba además que los 
 * datos recibidos corresponden al paquete al analizar el primer bit de cada
 * byte despúes del SYNC bit.
 * @return Retorna los paquetes de datos al arreglo @ref data 
 */
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

/** Decodifica y envía los datos del oxímetro de pulso al computador
 * @details Lee los datos desde el arreglo @ref data y decodifica los valores
 * según las especificaciones del fabricante del oxímetro de pulso.
 *
 * Decodifica tanto valores numéricos como de estado (Binarios), para luego
 * enviarlos en formato CSV al computador.
 * Los datos llevan la estructura:
 *
 * @code pulseIntensity,oxygenDissolved,pulseSound,pulseRate,oxygenSaturation,
 searchTimeTooLong,oxygenSaturationDecrease,probeError,searchPulse @endcode
 */
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

/** Convierte datos binarios a su representación Hexadecimal
 * @details Utiliza el arreglo @ref bin2hex para obtener la reprsentación
 * numerica hexadecimal de un número binario de 8 bits.
 * @param bin valor binario a convertir
 */
void serial_writeBin(uint8_t bin){
    serial_write(bin2hex[(bin&0xF0)>>4]);
    serial_write(bin2hex[bin&0x0F]);
}

/** Inicializa el puerto serial para la comunicación con el computador
 * @details Inicializa la librería LUFA para administrar la comunicación
 * serial entre el microcontrolador y el computador
 */
void LUFA_Init(void){
    // Enable LUFA
    serial_init();
}

/** Inicializa el puerto USART para la comunicación con el oxímetro de pulso
 * @details Configura el puerto USART para ser utilizado como puerto serial
 * 8N1, habilitando los registros necesarios
 * @param baud velocidad del puerto serial, obtenido por la tabla 18-12 del
 * datasheet del microcontrolador ATMega 32u4
 * @note Sólo se habilita la recepción de datos, no la transmición
 */
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

/** Recibe datos desde el oxímetro de pulso
 * @details Espera la recepción de un byte completo y la retorna
 * @return último byte recibido
 */
unsigned char USART_Receive(void){
    // Wait for data to be received
    while(!(UCSR1A & (1<<RXC1)));

    // Get and return received data from buffer
    return UDR1;
}