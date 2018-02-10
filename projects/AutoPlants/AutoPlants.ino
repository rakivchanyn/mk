
#include <Boards.h>
#include <Firmata.h>

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
//const int analogOutPin = 9; // Analog output pin that the LED is attached to

const int lightPin = LED_BUILTIN;
const int releyPin = 9;
const int switchSensorPin = 5;
const int switchPumpPin = 8;
const byte interruptPin = 3;
const int ledPin = 4;

int amountOfLight = 0;        // value read from the pot
//int outputValue = 0;        // value output to the PWM (analog out)
int isLightSwitched = 0;
int averageWithSwithedLight = 0;
int newThreshold = 0;
int timeFromWatering = 0;
int minutesOfLight = 0;
int day = 1;
volatile int delayTime = 60;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(lightPin, OUTPUT);
  pinMode(releyPin, OUTPUT);
  pinMode(switchSensorPin, OUTPUT);
  pinMode(switchPumpPin, OUTPUT);
  digitalWrite(releyPin, HIGH);
  digitalWrite(switchPumpPin, HIGH);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
//  pinMode(interruptPin, INPUT_PULLUP);
//  attachInterrupt(digitalPinToInterrupt(interruptPin), switchDistlay, LOW);
}

void delaySec( int iSec )
{
  while ( iSec )
  {
    delay(1000);
    --iSec;
  }
}

int averLight( int iIterNum, int iSecDelay )
{
  digitalWrite( switchSensorPin, HIGH );
  volatile int averageLight = analogRead(analogInPin) / iIterNum;
  delaySec(iSecDelay);
  for ( int i = 0; i < iIterNum - 1; ++i )
  {
    averageLight += (analogRead(analogInPin) / iIterNum );
    delaySec(iSecDelay);
  }
  digitalWrite( switchSensorPin, LOW );
  return averageLight;
}

void switchTheLightOn()
{
  digitalWrite(releyPin, LOW);
  isLightSwitched = 1;  
}

void switchTheLightOff()
{
  digitalWrite(releyPin, HIGH);
  isLightSwitched = 0;  
}


void loop()
{
  amountOfLight = averLight(15, 5);
  
  if ( !isLightSwitched && (amountOfLight < 150) && minOfLight < 840 )
  {
    switchTheLightOn();
    delaySec(15);
    newThreshold = averLight(15, 5);
  }
  else if ( ( isLightSwitched && (amountOfLight > (newThreshold + 150) ) ) || 
              ( averageLight < 150 && !isLightSwitched && minOfLight > 840 ) ) )   
  { 
    for (int i = 0; i < 15; ++i)
    {
      delaySec(60);
    }
//    amountOfLight = averLight(15, 5);
//      Serial.print("switch the light off ");
//      Serial.println(amountOfLight);
    switchTheLightOff();
  }
//  Serial.print("loop - "); 
//  Serial.println(amountOfLight);
//  digitalWrite(switchSensorPin, LOW);
//  timeFromWatering += 1;
//  Serial.print("TIME FROM WATERING "); 
//  Serial.println(timeFromWatering); 
 
  delaySec(delayTime);
}

