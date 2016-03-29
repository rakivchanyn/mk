#include <avr/io.h>
#include "uart.h"
#include "stdio.h"
#include "ADC.h"
#include "support.h"
#include <avr/interrupt.h>
#include <avr/power.h>

//void blinkRGB_B0_B1_B2(uint16_t del)
//{
//	PORTB |= 1<<PORTB0;
//	delay(delay);
//	PORTB ^= 1<<PORTB0;
//	PORTB |= 1<<PORTB1;
//	delay(delay);
//	PORTB ^= 1<<PORTB1;
//	PORTB |= 1<<PORTB2;
//	delay(delay);
//	PORTB ^= 1<<PORTB2;
//}
volatile uint16_t gMicroSec = 0;
volatile uint16_t g10MicroSec = 0;
volatile uint16_t gMiliSec = 0;
volatile uint16_t gSec = 0;

volatile uint8_t moveSensPin = 5;
volatile uint8_t ledPin = 7;

volatile uint8_t trigPin = 3;
volatile uint8_t echoPin = 4;
void setup()
{
	setPinMode(moveSensPin, 0);
	setPinMode(7, 1);
	setPinMode(echoPin, 0);
	setPinMode(trigPin, 1);
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
	if (g10MicroSec >= 10000)
	{
		g10MicroSec = 0;
		gMiliSec += 100;
	}
	if (gMiliSec >= 1000)
	{
		gMiliSec = 0;
		++gSec;
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

void delayX10MicroSec (uint16_t iMicroSec)
{
	g10MicroSec = 0;
	while ((g10MicroSec) < iMicroSec) doNothing();
}

#define MAX_TIME 50000

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

int main ()
{
// 8-bit Timer interrupt init----------------------------------------------------
//	TCNT0=0;
////	Place TOP timer values to Output compare registers
//	OCR0A = 249;
//	TCCR0A |= (1<<COM0A0)|(1<<WGM01);
//	//Enable Timer0 OCF0B Interrupt
//	TIMSK0 |= (1<<OCIE0A);
////	sei();
//	TCCR0B |= (1 << CS01) | (1 << CS00);
//----------------------------------------------------------

	//Place TOP timer 2 values to Output compare registers
//	OCR2A = 15;
//	TCCR2A |= (1<<WGM21);
//
//	TIMSK2 |= (1<<OCIE2A);
//	sei();
//	TCCR2B = (1 << CS20);
//----------------------------------------------------------

	TCNT1=0;
	OCR1A = 159;
	//First capture on rising edge
	TCCR1B |= (1<<WGM12);
	//Enable input capture and overflow interrupts
	TIMSK1 = (1<<OCIE1A);
//	TIFR1 |= (1<<OCF1B);
	//Start timer without prescaller
	TCCR1B |= (1<<CS10);
	//Enable global interrutps
	sei();

//----------------------------------------------------------

	setup();
	uint16_t switchLight = 0;
	uint16_t manuallySwitched = 0;
//	InitADC();
	USART_Init(MYUBRR);
	while(1)
	{
		gSec = 0;

		if (isThereInteraction(25))
		{
			manuallySwitched = 1;
			switchLight = ~switchLight;
			digitalWrite(ledPin, switchLight);
		}

		volatile uint16_t val2 = 1;// PIND & 1<<PIND5; // motion sensor date
		if ( val2 )
		{
			digitalWrite(ledPin, manuallySwitched ? switchLight : 1);
			while ( gSec < 100 )
			{
				volatile uint16_t move = PIND & 1<<PIND5; //don't switch off until movements present or switch off manually
				if ( move )
				{
					gSec = 0;
				}

				if (isThereInteraction(25))
				{
					manuallySwitched = 1;
					switchLight = ~switchLight;
					digitalWrite(ledPin, switchLight);
				}
			}
		}
		else
		{
			gSec = 0;
			manuallySwitched = 0;
			switchLight = 0;
			digitalWrite(ledPin, 0);
		}

		delayX10MicroSec(100);
	}
	return 0;
}

//DDRB |= 1<<DDB0;
//DDRB |= 1<<DDB1;
//DDRB |= 1<<DDB2;
//DDRB |= 1<<DDB3;
//DDRB |= 1<<DDB4;
//DDRB |= 1<<DDB5;
//
//DDRD |= 1<<DDD2;
//DDRD |= 1<<DDD3;
//DDRD |= 1<<DDD4;
//DDRD |= 1<<DDD5;
//DDRD |= 1<<DDD6;
//DDRD |= 1<<DDD7;
