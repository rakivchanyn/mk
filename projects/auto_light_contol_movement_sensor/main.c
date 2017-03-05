#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/io.h>
#include "uart.h"
#include "ADC.h"
#include "support.h"
#include "LightControl.h"
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

int main ()
{
	PlantsGrowAutomation();
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
