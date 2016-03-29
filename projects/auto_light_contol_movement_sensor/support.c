/*
 * support.c
 *
 *  Created on: 27 лют. 2016 р.
 *      Author: Gumenyak
 */
#include "support.h"
#include "avr/io.h"

void delay(uint16_t msec)
{
	volatile uint16_t count = 0;
	while (count < msec)
	{
		volatile uint16_t count2 = 0;
		while (count2 < 1000)
			++count2;

		++count;
	}
}

void to_num16(char * buf, uint16_t num)
{
    int rem;
    int buf_index = 0;

    do
    {
        rem = num % 10;
        num /= 10;
        buf[buf_index++] = rem + '0';
    } while(num);

    //now reverse the order
    for(int i = 0; i < buf_index/2; ++i)
    {
        char t = buf[i];
//        buf[i] = buf[buf_index-1-t];
//        buf[buf_index-1-t] = t;
        buf[i] = buf[buf_index-1-i];
        buf[buf_index-1-i] = t;
    }
    buf[buf_index] = 0;
}

void setPinMode(uint8_t iPin, uint8_t iMode)
{
	switch (iPin)
	{
		case 0:  if (iMode == 0) DDRD &= ~(1<<DDD0); else DDRD |= 1<<DDD0;	break;
		case 1:  if (iMode == 0) DDRD &= ~(1<<DDD1); else DDRD |= 1<<DDD1;	break;
		case 2:  if (iMode == 0) DDRD &= ~(1<<DDD2); else DDRD |= 1<<DDD2;	break;
		case 3:  if (iMode == 0) DDRD &= ~(1<<DDD3); else DDRD |= 1<<DDD3;	break;
		case 4:  if (iMode == 0) DDRD &= ~(1<<DDD4); else DDRD |= 1<<DDD4;	break;
		case 5:  if (iMode == 0) DDRD &= ~(1<<DDD5); else DDRD |= 1<<DDD5;	break;
		case 6:  if (iMode == 0) DDRD &= ~(1<<DDD6); else DDRD |= 1<<DDD6;	break;
		case 7:  if (iMode == 0) DDRD &= ~(1<<DDD7); else DDRD |= 1<<DDD7;	break;
		case 8:  if (iMode == 0) DDRB &= ~(1<<DDB0); else DDRB |= 1<<DDB0;	break;
		case 9:	 if (iMode == 0) DDRB &= ~(1<<DDB1); else DDRB |= 1<<DDB1;	break;
		case 10: if (iMode == 0) DDRB &= ~(1<<DDB2); else DDRB |= 1<<DDB2;	break;
		case 11: if (iMode == 0) DDRB &= ~(1<<DDB3); else DDRB |= 1<<DDB3;	break;
		case 12: if (iMode == 0) DDRB &= ~(1<<DDB4); else DDRB |= 1<<DDB4;	break;
		case 13: if (iMode == 0) DDRB &= ~(1<<DDB5); else DDRB |= 1<<DDB5;	break;
		case 14:	break;
		default:	break;
	}
}

void digitalWrite (uint8_t iPin, uint8_t iVal)
{
	switch (iPin)
	{
		case 0:  if (iVal == 0) PORTD &= ~(1<<PORTD0); else PORTD |= 1<<PORTD0;	break;
		case 1:  if (iVal == 0) PORTD &= ~(1<<PORTD1); else PORTD |= 1<<PORTD1;	break;
		case 2:  if (iVal == 0) PORTD &= ~(1<<PORTD2); else PORTD |= 1<<PORTD2;	break;
		case 3:  if (iVal == 0) PORTD &= ~(1<<PORTD3); else PORTD |= 1<<PORTD3;	break;
		case 4:  if (iVal == 0) PORTD &= ~(1<<PORTD4); else PORTD |= 1<<PORTD4;	break;
		case 5:  if (iVal == 0) PORTD &= ~(1<<PORTD5); else PORTD |= 1<<PORTD5;	break;
		case 6:  if (iVal == 0) PORTD &= ~(1<<PORTD6); else PORTD |= 1<<PORTD6;	break;
		case 7:  if (iVal == 0) PORTD &= ~(1<<PORTD7); else PORTD |= 1<<PORTD7;	break;
		case 8:  if (iVal == 0) PORTB &= ~(1<<PORTB0); else PORTB |= 1<<PORTB0;	break;
		case 9:	 if (iVal == 0) PORTB &= ~(1<<PORTB1); else PORTB |= 1<<PORTB1;	break;
		case 10: if (iVal == 0) PORTB &= ~(1<<PORTB2); else PORTB |= 1<<PORTB2;	break;
		case 11: if (iVal == 0) PORTB &= ~(1<<PORTB3); else PORTB |= 1<<PORTB3;	break;
		case 12: if (iVal == 0) PORTB &= ~(1<<PORTB4); else PORTB |= 1<<PORTB4;	break;
		case 13: if (iVal == 0) PORTB &= ~(1<<PORTB5); else PORTB |= 1<<PORTB5;	break;
		case 14:	break;
		default:	break;
	}
}
//
uint8_t digitalRead (uint8_t iPin)
{
//    if (*portInputRegister(port) & bit) return HIGH;
//    return LOW;
}
