#include "avr/io.h"
#include "uart.h"
#include "7segment4digitsDisplay.h"
#include "stdio.h"

void _delay_ms(uint16_t msec)
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

void InitADC()
{
	// Select Vref=AVcc
	ADMUX |= (1<<REFS0);
	//set prescaller to 128 and enable ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}

uint16_t ReadADC(uint8_t ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	return ADC;
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

void blinkRGB_B0_B1_B2(uint16_t delay)
{
	PORTB |= 1<<PORTB0;
	_delay_ms(delay);
	PORTB ^= 1<<PORTB0;
	PORTB |= 1<<PORTB1;
	_delay_ms(delay);
	PORTB ^= 1<<PORTB1;
	PORTB |= 1<<PORTB2;
	_delay_ms(delay);
	PORTB ^= 1<<PORTB2;
}


//void digitalWrite (uint8_t pin, uint8_t val)
//{
//	switch (pin)
//	{
//	case 0: if (val == 0) PORTD &= 1<<PORTD0; else PORTD |= 1<<PORTD0;	break;
//	case 1: if (val == 0) PORTD &= 1<<PORTD1; else PORTD |= 1<<PORTD1;	break;
//	case 2: if (val == 0) PORTD &= 1<<PORTD2; else PORTD |= 1<<PORTD2;	break;
//	case 3: if (val == 0) PORTD &= 1<<PORTD3; else PORTD |= 1<<PORTD3;	break;
//	case 4: if (val == 0) PORTD &= 1<<PORTD4; else PORTD |= 1<<PORTD4;	break;
//	case 5: if (val == 0) PORTD &= 1<<PORTD5; else PORTD |= 1<<PORTD5;	break;
//	case 6: if (val == 0) PORTD &= 1<<PORTD6; else PORTD |= 1<<PORTD6;	break;
//	case 7: if (val == 0) PORTD &= 1<<PORTD7; else PORTD |= 1<<PORTD7;	break;
//	case 8:  if (val == 0) PORTB &= 1<<PORTB0; else PORTB |= 1<<PORTB0;	break;
//	case 9:	 if (val == 0) PORTB &= 1<<PORTB1; else PORTB |= 1<<PORTB1;	break;
//	case 10: if (val == 0) PORTB &= 1<<PORTB2; else PORTB |= 1<<PORTB2;	break;
//	case 11: if (val == 0) PORTB &= 1<<PORTB3; else PORTB |= 1<<PORTB3;	break;
//	case 12: if (val == 0) PORTB &= 1<<PORTB4; else PORTB |= 1<<PORTB4;	break;
//	case 13: if (val == 0) PORTB &= 1<<PORTB5; else PORTB |= 1<<PORTB5;	break;
//	case 14:	break;
//	default:	break;
//
//	}
//}

void setup()
{
	DDRB |= 1<<DDB0;
	DDRB |= 1<<DDB1;
	DDRB |= 1<<DDB2;
	DDRB |= 1<<DDB3;
	DDRB |= 1<<DDB4;
	DDRB |= 1<<DDB5;

	DDRD |= 1<<DDD2;
	DDRD |= 1<<DDD3;
	DDRD |= 1<<DDD4;
	DDRD |= 1<<DDD5;
	DDRD |= 1<<DDD6;
	DDRD |= 1<<DDD7;
}

int main ()
{
	setup();
	InitADC();
	USART_Init(MYUBRR);
	while(1)
	{
//		_delay_ms(3000);
		volatile uint16_t adc = ReadADC(0);
		float tempr = (adc * 0.4883);
		//printFloat(tempr);

		int i = 0;
		while (i < 100)
		{
			displayFloatNumber(tempr);
			++i;
		}
	}
	return 0;
}
