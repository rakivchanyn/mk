/*
 * 7segment4digitsDisplay.c
 *
 *  Created on: 24 ñ³÷. 2016 ð.
 *      Author: Gumenyak
 */
#include "avr/io.h"
#include "uart.h"
#include "7segment4digitsDisplay.h"
#include "support.h"

int aPin = 5;  //                A
int bPin = 13; //            __________
int cPin = 10; //           |          |
int dPin = 8;  //       F   |          |  B
int ePin = 7;  //           |    G     |
int fPin = 4;  //           |__________|
int gPin = 11; //           |          |
int GND1 = 6;  //           |          |
int GND2 = 3;  //       E   |          |   C
int GND3 = 2;  //           |__________|
int GND4 = 12; //
int POINT = 9; //                D
int dig1 = 0;
int dig2 = 0;
int dig3 = 0;
int dig4 = 0;
int DTime = 3;

void delay_ms(uint16_t msec)
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

void clearLEDs()
{
  digitalWrite( aPin, 0);
  digitalWrite( bPin, 0);
  digitalWrite( cPin, 0);
  digitalWrite( dPin, 0);
  digitalWrite( ePin, 0);
  digitalWrite( fPin, 0);
  digitalWrite( gPin, 0);
}

void one()
{
  digitalWrite( aPin, 0);
  digitalWrite( bPin, 1);
  digitalWrite( cPin, 1);
  digitalWrite( dPin, 0);
  digitalWrite( ePin, 0);
  digitalWrite( fPin, 0);
  digitalWrite( gPin, 0);
}

void two()
{
  digitalWrite( aPin, 1);
  digitalWrite( bPin, 1);
  digitalWrite( cPin, 0);
  digitalWrite( dPin, 1);
  digitalWrite( ePin, 1);
  digitalWrite( fPin, 0);
  digitalWrite( gPin, 1);
}

void three()
{
  digitalWrite( aPin, 1);
  digitalWrite( bPin, 1);
  digitalWrite( cPin, 1);
  digitalWrite( dPin, 1);
  digitalWrite( ePin, 0);
  digitalWrite( fPin, 0);
  digitalWrite( gPin, 1);
}

void four()
{
  digitalWrite( aPin, 0);
  digitalWrite( bPin, 1);
  digitalWrite( cPin, 1);
  digitalWrite( dPin, 0);
  digitalWrite( ePin, 0);
  digitalWrite( fPin, 1);
  digitalWrite( gPin, 1);
}

void five()
{
  digitalWrite( aPin, 1);
  digitalWrite( bPin, 0);
  digitalWrite( cPin, 1);
  digitalWrite( dPin, 1);
  digitalWrite( ePin, 0);
  digitalWrite( fPin, 1);
  digitalWrite( gPin, 1);
}

void six()
{
  digitalWrite( aPin, 1);
  digitalWrite( bPin, 0);
  digitalWrite( cPin, 1);
  digitalWrite( dPin, 1);
  digitalWrite( ePin, 1);
  digitalWrite( fPin, 1);
  digitalWrite( gPin, 1);
}

void seven()
{
  digitalWrite( aPin, 1);
  digitalWrite( bPin, 1);
  digitalWrite( cPin, 1);
  digitalWrite( dPin, 0);
  digitalWrite( ePin, 0);
  digitalWrite( fPin, 0);
  digitalWrite( gPin, 0);
}

void eight()
{
  digitalWrite( aPin, 1);
  digitalWrite( bPin, 1);
  digitalWrite( cPin, 1);
  digitalWrite( dPin, 1);
  digitalWrite( ePin, 1);
  digitalWrite( fPin, 1);
  digitalWrite( gPin, 1);
}

void nine()
{
  digitalWrite( aPin, 1);
  digitalWrite( bPin, 1);
  digitalWrite( cPin, 1);
  digitalWrite( dPin, 1);
  digitalWrite( ePin, 0);
  digitalWrite( fPin, 1);
  digitalWrite( gPin, 1);
}

void zero()
{
  digitalWrite( aPin, 1);
  digitalWrite( bPin, 1);
  digitalWrite( cPin, 1);
  digitalWrite( dPin, 1);
  digitalWrite( ePin, 1);
  digitalWrite( fPin, 1);
  digitalWrite( gPin, 0);
}

void pickNumber(int x){
   switch(x){
     case 1: one(); break;
     case 2: two(); break;
     case 3: three(); break;
     case 4: four(); break;
     case 5: five(); break;
     case 6: six(); break;
     case 7: seven(); break;
     case 8: eight(); break;
     case 9: nine(); break;
     default: zero(); break;
   }
}

void displayFloatNumber(float num)
{
	digitalWrite( GND1, 1);
	digitalWrite( GND2, 1);
	digitalWrite( GND3, 1);
	digitalWrite( GND4, 1);

	uint16_t val = (uint16_t) (num * 100);

	dig1 = val / 1000;
	val = val - (dig1 * 1000);
	dig2 = val / 100;
	val = val - (dig2 * 100);
	dig3 = val / 10;
	dig4 = val - (dig3 *10);

	digitalWrite( GND4, 0);    //digit 4
	pickNumber(dig4);
	delay_ms(DTime);
	digitalWrite( GND4, 1);

	digitalWrite( GND3, 0);    //digit 3
	pickNumber(dig3);
	delay_ms(DTime);
	digitalWrite( GND3, 1);

	digitalWrite( GND2, 0);   //digit 2
	pickNumber(dig2);
	digitalWrite(POINT, 1);
	delay_ms(DTime);
	digitalWrite(POINT, 0);
	digitalWrite( GND2, 1);

	digitalWrite( GND1, 0);   //digit 1
	pickNumber(dig1);
	delay_ms(DTime);
	digitalWrite( GND1, 1);
}

void displayNumber(int num)
{
//	printFloat(1.0f);
	digitalWrite( GND1, 1);
	digitalWrite( GND2, 1);
	digitalWrite( GND3, 1);
	digitalWrite( GND4, 1);

	dig1 = num / 1000;
	num = num - (dig1 * 1000);
	dig2 = num / 100;
	num = num - (dig2 * 100);
	dig3 = num / 10;
	dig4 = num - (dig3 *10);

	digitalWrite( GND4, 0);    //digit 4
	pickNumber(dig4);
	delay_ms(DTime);
	digitalWrite( GND4, 1);

	digitalWrite( GND3, 0);    //digit 3
	pickNumber(dig3);
	delay_ms(DTime);
	digitalWrite( GND3, 1);

	digitalWrite( GND2, 0);   //digit 2
	pickNumber(dig2);
	delay_ms(DTime);
	digitalWrite( GND2, 1);

	digitalWrite( GND1, 0);   //digit 1
	pickNumber(dig1);
	delay_ms(DTime);
	digitalWrite( GND1, 1);

//	PORTD |= 1<<PORTD5;
//	delay_ms(1000);
//	PORTD &= ~(1<<PORTD5);
//	delay_ms(1000);
}
