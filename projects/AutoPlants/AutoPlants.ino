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

int amountOfLight = 0;        // value read from the pot
//int outputValue = 0;        // value output to the PWM (analog out)
int isLightSwitched = 0;
int averageWithSwithedLight = 0;
int newThreshold = 0;
int timeFromWatering = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(lightPin, OUTPUT);
  pinMode(releyPin, OUTPUT);
  pinMode(switchSensorPin, OUTPUT);
  pinMode(switchPumpPin, OUTPUT);
  digitalWrite(releyPin, HIGH);
  digitalWrite(switchPumpPin, HIGH);

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
    volatile int averageLight = analogRead(analogInPin) / iIterNum;
    delaySec(iSecDelay);
    for ( int i = 0; i < iIterNum - 1; ++i )
    {
      averageLight += (analogRead(analogInPin) / iIterNum );
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
  delaySec(15);
  digitalWrite(switchPumpPin, LOW);
  delaySec(iTimeSec);
  digitalWrite(switchPumpPin, HIGH);
  timeFromWatering = 0;
}

void loop() {
  lcd.backlight();
  lcd.display();
  
  digitalWrite ( switchSensorPin, HIGH );
  amountOfLight = averLight(5, 5);
  
  if ( !isLightSwitched && (amountOfLight < 150) )
  {
    lcd.clear();
    lcd.home();
    lcd.print("switch on");
    lcd.setCursor( 0, 1 );
    lcd.print("light - ");
    lcd.print(amountOfLight);
    delay(3000);    
//    Serial.print("switch the light on ");
//    Serial.println(amountOfLight);

    switchTheLight( 1 );

    lcd.clear();
    lcd.home();
    lcd.print("watering");
    watering(30);
    
    lcd.clear();
    lcd.home();
    lcd.print("calc. threshold");
    newThreshold = averLight(15, 5);
    
    lcd.clear();
    lcd.home();
    lcd.print("Threshold ");
    lcd.setCursor ( 0, 1 );
    lcd.print(newThreshold);
    delay(3000);
    
//    Serial.print("newThreshold ");
//    Serial.println(newThreshold);
  }
  else if ( isLightSwitched && (amountOfLight > (newThreshold + 100)) )   
  {
    for (int i = 0; i < 12; ++i)
    {
      lcd.clear();
      lcd.home();
      lcd.print("to switch off");
      lcd.setCursor ( 0, 1 );
      lcd.print((12 - i) * 5);
      lcd.print(" min");
      delaySec(5 * 60);
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
  lcd.noDisplay();
  lcd.noBacklight();
//  Serial.print("TIME FROM WATERING "); 
//  Serial.println(timeFromWatering);  
  delaySec(10);
}
