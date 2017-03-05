/*
 * LightControl.c
 *
 *  Created on: 1 ���. 2016 �.
 *      Author: Gumenyak
 */

#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include "libs/ds1302_clock/DS1302.h"
#include "Arduino.h"

#include "PlantsGrowAutomation.h"
#include "support/support.h"
#include "support/uart.h"

//#define USE_US_SENSOR

#define MAX_TIME 50000

volatile uint16_t gMicroSec = 0;
volatile uint16_t g10MicroSec = 0;
volatile uint16_t g10MicroSec2 = 0;
volatile uint16_t gMiliSec = 0;
volatile uint16_t gSec = 0;
volatile uint16_t gSec2 = 0;
volatile uint16_t gForgotInterSec = 0;

volatile uint16_t gSwitchLight = 0;
volatile uint16_t gManuallySwitched = 0;
volatile uint16_t gCanBeSwitched = 1;
volatile uint16_t gIgnoreMotion = 0;

volatile uint8_t moveSensPin =  5;
volatile uint8_t ledPin = 7;
volatile uint8_t highLightPin = 8;
volatile uint8_t ledTest = 13;

volatile uint8_t trigPin = 3;
volatile uint8_t echoPin = 4;

void setup()
{
	setPinMode(moveSensPin, 0);
	setPinMode(ledPin, 1);
	setPinMode(echoPin, 0);
	setPinMode(trigPin, 1);
	setPinMode(highLightPin, 1);
	setPinMode(ledTest, 1);

	startTimer1_16bit(159);
}

void startTimer1_16bit(uint8_t iInteraptValue)
{
	TCNT1 = 0;
	OCR1A = iInteraptValue; //159
	//First capture on rising edge
	TCCR1B |= (1<<WGM12);
	//Enable input capture and overflow interrupts
	TIMSK1 = (1<<OCIE1A);
	//	TIFR1 |= (1<<OCF1B);
	//Start timer without prescaller
	TCCR1B |= (1<<CS10);
	//Enable global interrutps
	sei();
}

ISR(TIMER1_COMPA_vect)
{
	++g10MicroSec;
	++g10MicroSec2;
	if (g10MicroSec >= 10000)
	{
		g10MicroSec = 0;
		gMiliSec += 100;
	}
	if (gMiliSec >= 1000)
	{
		gMiliSec = 0;
		++gSec;
		++gSec2;
		++gForgotInterSec;
	}
}

void doNothing()
{
	volatile uint16_t i = 0;
	i++;
}

void delayX10MicroSec(uint16_t iMicroSec)
{
	g10MicroSec2 = 0;
	while (g10MicroSec2 < iMicroSec) doNothing();
}

void delaySec(uint16_t iSec)
{
	gSec2 = 0;
	while ( gSec2 < iSec ) doNothing();
}

uint16_t timeTravel (uint16_t iPin, uint16_t iState)
{
	volatile uint16_t i = 0;
	g10MicroSec = 0;
	while ( !(PIND & 1<<PIND4) )
		++i;

	uint16_t msec = 0;
	while ( (PIND & (1<<PIND4)) && (g10MicroSec < MAX_TIME) )
		msec = g10MicroSec;

	return (msec * 10);
}

void delayX4MicroSeconds( unsigned int us)
{
	// for the 16 MHz clock on most Arduino boards

	// for a one-microsecond delay, simply return.  the overhead
	// of the function call takes 14 (16) cycles, which is 1us
	if (us <= 1) return; //  = 3 cycles, (4 when true)

	// the following loop takes 1/4 of a microsecond (4 cycles)
	// per iteration, so execute it four times for each microsecond of
	// delay requested.
	us <<= 2; // x4 us, = 4 cycles

	// account for the time taken in the preceeding commands.
	// we just burned 19 (21) cycles above, remove 5, (5*4=20)
	// us is at least 8 so we can substract 5
	us -= 2; // = 2 cycles,
	// busy wait
	asm volatile (
		"1: sbiw %0,1" "\n\t" // 2 cycles
		"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
	);
}

void PlantsGrowAutomation()
{
	setup();
//	USART_Init(MYUBRR);

	while(1)
	{

		digitalWrite(ledTest, 0);
		delaySec(1);
		digitalWrite(ledTest, 1);
		delaySec(1);



	}
}
