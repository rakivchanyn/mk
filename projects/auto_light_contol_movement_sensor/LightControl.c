/*
 * LightControl.c
 *
 *  Created on: 1 квіт. 2016 р.
 *      Author: Gumenyak
 */

#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include "LightControl.h"
#include "support.h"
#include "uart.h"

#define MAX_TIME 50000

volatile uint16_t gMicroSec = 0;
volatile uint16_t g10MicroSec = 0;
volatile uint16_t g10MicroSec2 = 0;
volatile uint16_t gMiliSec = 0;
volatile uint16_t gSec = 0;
volatile uint16_t gSec2 = 0;

volatile uint16_t gSwitchLight = 0;
volatile uint16_t gManuallySwitched = 0;
volatile uint16_t gCanBeSwitched = 0;
volatile uint16_t gIgnoreMotion = 0;

volatile uint8_t moveSensPin = 6;
volatile uint8_t ledPin = 7;

volatile uint8_t trigPin = 3;
volatile uint8_t echoPin = 4;

void setup()
{
	setPinMode(moveSensPin, 0);
	setPinMode(ledPin, 1);
	setPinMode(echoPin, 0);
	setPinMode(trigPin, 1);

	startTimer1_16bit(159);
	//InitADC();
//	USART_Init(MYUBRR);
}

void startTimer0_8bit(uint8_t iInteraptValue)
{
	TCNT0 = 0;
	//	Place TOP timer values to Output compare registers
	OCR0A = iInteraptValue; //249
	TCCR0A |= (1<<COM0A0)|(1<<WGM01);
	//Enable Timer0 OCF0B Interrupt
	TIMSK0 |= (1<<OCIE0A);
	sei();
	TCCR0B |= (1 << CS01) | (1 << CS00);
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

void startTimer2_8bit(uint8_t iInteraptValue)
{
	TCNT2 = 0;
	OCR2A = iInteraptValue; //15
	TCCR2A |= (1<<WGM21);
	TIMSK2 |= (1<<OCIE2A);
	sei();
	TCCR2B = (1 << CS20);
}


//ISR(TIMER0_COMPA_vect)
//{
//	++gMiliSec;
//	if (gMiliSec >= 1000)
//	{
//		gMiliSec = 0;
//		++gSec;
//		printToPort("sec timer 0");
//	}
//}

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
	}
}
//
//ISR(TIMER2_COMPA_vect)
//{
//	++gMicroSecUS;
//	if ( gMicroSecUS >= 1000 )
//	{
//		gMicroSecUS = 0;
//		++gMiliS;
//	}
//}

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

uint16_t isThereInteraction(uint16_t iInetactionDist)
{
	digitalWrite(trigPin, 0);
	delayX4MicroSeconds(1);
	digitalWrite(trigPin, 1);
	delayX4MicroSeconds(3);
	digitalWrite(trigPin, 0);

	volatile uint16_t time = timeTravel(echoPin, 1);
	volatile uint16_t dist = time*0.034/2;
	return (dist < iInetactionDist);
}

void switchLight()
{
	if (isThereInteraction(25) && gCanBeSwitched)
	{
		gManuallySwitched = 1;
		gCanBeSwitched = 0;
		gSwitchLight = ~gSwitchLight;
		digitalWrite(ledPin, gSwitchLight);
		delaySec(3);
	}
	else
	{
		gCanBeSwitched = 1;
		delayX10MicroSec(100);
	}
}

void lightControl()
{
	setup();
	USART_Init(MYUBRR);

	while(1)
	{
		switchLight();

		volatile uint16_t motionDetected = PIND & 1<<PIND6; // motion sensor date
		if ( motionDetected )
		{
			printToPort("inside motion loop\n");
			digitalWrite(ledPin, gManuallySwitched ? gSwitchLight : 1);
			gSec = 0;
			while ( gSec < 60 )
			{
				volatile uint16_t move = PIND & 1<<PIND5; //don't switch off until movements present or switch off manually
				if ( move )
				{
					gSec = 0;
				}
				switchLight();
			}
		}
		else if(!gManuallySwitched)
		{
			gManuallySwitched = 0;
			gSwitchLight = 0;
			digitalWrite(ledPin, 0);
		}

		delayX10MicroSec(1000);
		gSec = 0;
	}
}
