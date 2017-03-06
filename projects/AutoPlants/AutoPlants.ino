#include <Boards.h>
#include <Firmata.h>
#include <LiquidCrystal_I2C.h>

#define BACKLIGHT_PIN     13

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// Creat a set of new characters
const uint8_t charBitmap[][8] = {
   { 0xc, 0x12, 0x12, 0xc, 0, 0, 0, 0 },
   { 0x6, 0x9, 0x9, 0x6, 0, 0, 0, 0 },
   { 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0, 0x0 },
   { 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0, 0x0 },
   { 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0x0 },
   { 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0x0 },
   { 0x0, 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0x0 },
   { 0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0x0 }
   
};



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
volatile bool state = false;
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
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), switchDistlay, LOW);

  int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));

  // Switch on the backlight
  pinMode ( BACKLIGHT_PIN, OUTPUT );
  digitalWrite ( BACKLIGHT_PIN, HIGH );
  
  lcd.begin(16,2);               // initialize the lcd 

  for ( int i = 0; i < charBitmapSize; i++ )
  {
     lcd.createChar ( i, (uint8_t *)charBitmap[i] );
  }
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
    if (state)
    {
      lcd.clear();
      lcd.home();
      lcd.print("calc. avr. light");
      lcd.setCursor( 0, 1 );
      lcd.print(iIterNum * iSecDelay);
      lcd.print(" sec");
    }
    volatile int averageLight = analogRead(analogInPin) / iIterNum;
    delaySec(iSecDelay);
    for ( int i = 0; i < iIterNum - 1; ++i )
    {
      averageLight += (analogRead(analogInPin) / iIterNum );
      if (state)
      {
        lcd.clear();
        lcd.home();
        lcd.print("calc. avr. light");
        lcd.setCursor( 0, 1 );
        lcd.print(((iIterNum - 1) - i) * iSecDelay);
        lcd.print(" sec");
      }
      delaySec(iSecDelay);
    }
//    lcd.home();
//    lcd.print("Average light - ");
//    lcd.setCursor ( 0, 1 );
//    lcd.print("value - "); 
//    lcd.print(averageLight);
//    delay(3000);
//    Serial.print("average light - ");
//    Serial.println(averageLight);
    return averageLight;
}

void switchTheLight(int iIsSwitchOn)
{
//  digitalWrite(lightPin, iIsSwitchOn ? HIGH : LOW);
  digitalWrite(releyPin, iIsSwitchOn ? LOW : HIGH);
  isLightSwitched = iIsSwitchOn;  
}

void watering(int iTimeSec)
{
  if (timeFromWatering < 500)
    return;
  
  if (state)
  {
    lcd.clear();
    lcd.home();
    lcd.print("watering");
  }
  delaySec(15);
  digitalWrite(switchPumpPin, LOW);
  delaySec(iTimeSec);
  digitalWrite(switchPumpPin, HIGH);
  timeFromWatering = 0;
}

void loop() {
  if (state)
  {
    lcd.backlight();
    lcd.display();
    Serial.println("state == true");
  }
  else
  {
    lcd.noDisplay();
    lcd.noBacklight();
    Serial.println("state == false");
  }

  digitalWrite ( switchSensorPin, HIGH );
  amountOfLight = averLight(15, 5);
  
  if ( !isLightSwitched && (amountOfLight < 150) )
  {
    lcd.backlight();
    lcd.display();
    state = true;
    if (state)
    {
      lcd.clear();
      lcd.home();
      lcd.print("switch on");
      lcd.setCursor( 0, 1 );
      lcd.print("light - ");
      lcd.print(amountOfLight);
      delay(3000);
    }    
//    Serial.print("switch the light on ");
//    Serial.println(amountOfLight);

    switchTheLight( 1 );

    watering(30);

    if (state)
    {
      lcd.clear();
      lcd.home();
      lcd.print("calc. threshold");
    }
    newThreshold = averLight(15, 5);

    if (state)
    {
      lcd.clear();
      lcd.home();
      lcd.print("Threshold ");
      lcd.setCursor ( 0, 1 );
      lcd.print(newThreshold);
      delay(3000);
    }
    lcd.noDisplay();
    lcd.noBacklight();
    state = false;
//    Serial.print("newThreshold ");
//    Serial.println(newThreshold);
  }
  else if ( isLightSwitched && (amountOfLight > (newThreshold + 150)) )   
  {
    for (int i = 0; i < 60; ++i)
    {
      if (state)
      {
        lcd.clear();
        lcd.home();
        lcd.print("to switch off");
        lcd.setCursor ( 0, 1 );
        lcd.print(60 - i);
        lcd.print(" min");
      }
      delaySec(60);
    }
    amountOfLight = averLight(15, 5);
//    if ( amountOfLight > 900 )
//    {
//      Serial.print("switch the light off ");
//      Serial.println(amountOfLight);
      switchTheLight( 0 );
//    }
  }
//  Serial.print("loop - "); 
//  Serial.println(amountOfLight);
  digitalWrite(switchSensorPin, LOW);
  timeFromWatering += 1;

  if (state)
  {
    lcd.clear();
    lcd.home();
    lcd.print("FROM WATERING ");
    lcd.setCursor ( 0, 1 );
    lcd.print(timeFromWatering);
    delay(3000);
    lcd.clear();
    lcd.home();
    lcd.print("Light - ");
    lcd.print(amountOfLight);
    lcd.setCursor ( 0, 1 );
    lcd.print("Threshold - ");
    lcd.print(newThreshold);
    delay(5000);
  }
//  Serial.print("TIME FROM WATERING "); 
//  Serial.println(timeFromWatering);  
  delaySec(delayTime);
}

void switchDistlay()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 20ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
  {
    state = !state;
    if (state)
    {  
      digitalWrite(ledPin, HIGH);
      delayTime = 1;
    }  
    else
    {
      digitalWrite(ledPin, LOW);
      delayTime = 60;
    }
  }
  last_interrupt_time = interrupt_time;
}

