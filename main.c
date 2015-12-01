#include <iom328p.h>
#include <avr_macros.h>

void delay (unsigned int iDelay)
{
  volatile int i = 0;
  while ( i < iDelay )
  {
    ++i;
  }
}

int main( void )
{
  DDRB_DDB0 = 0;
  DDRB_DDB1 = 1;
  while ( 1 )
  {
//    if ( PORTB_PORTB0 == 0 )
    {
      PORTB_PORTB1 ^= 1;
      delay( 50000 );
    }

  }
  return 0;
}
