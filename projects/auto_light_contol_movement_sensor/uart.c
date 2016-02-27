#include "uart.h"

void intToChar(char* buf, uint16_t in)
{
	volatile uint16_t b = 10000; //not more than 65535
	volatile uint16_t i = 0;
	while (b >= 1)
	{
		if ( in >= b )
		{
			volatile uint16_t temp = (uint16_t)(in/b);
			in = in - temp * b;
			buf[i] = temp + '0';
			i++;
		}
		b /= 10;
	}
	buf[i] = 0;
}

void USART_Init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void printToPort(char* string)
{
	while (*string != '\0')
	{
		USART_Transmit(*string);
		++string;
	}
}

void printFloat(float a)
{
	uint16_t val = (uint16_t)(a * 1000);
	uint16_t intPart = val / 1000;
	char intPartStr[10];
	intToChar(intPartStr, intPart);
	printToPort(intPartStr);
	printToPort(".");
	uint16_t digPart = val - intPart * 1000;
	char digPartStr[10];
	intToChar(digPartStr, digPart);
	printToPort(digPartStr);
}
