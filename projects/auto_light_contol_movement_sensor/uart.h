/*
 * uart.h
 *
 * UART example for ATMega328P clocked at 16 MHz
 *
 * TODO :-
 * 	- Implement string read function
 * 	- Optimize for size
 * 	- Add helper routines and compile to .a file
 *
 *  Created on: 22-Jan-2014
 *      Author: Shrikant Giridhar
 */

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

#define FOSC 16000000UL // Clock Speed
#define BAUD 9600
#define MYUBRR (FOSC/16)/BAUD-1
void intToChar(char* buf, uint16_t in);
void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
void printToPort(char* string);
void printInt(uint16_t i);
void printFloat(float a);
#endif /* UART_H_ */
