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
    Serial.print("average light - ");
    Serial.println(averageLight);
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
  // read the analog in value:

  amountOfLight = averLight(15, 5);
  if ( !isLightSwitched && (amountOfLight < 150) )
  {
    Serial.print("switch the light on ");
    Serial.println(amountOfLight);
    switchTheLight( 1 );
    watering(30);
    newThreshold = averLight(15, 5);
    Serial.print("newThreshold ");
    Serial.println(newThreshold);
  }
  else if ( isLightSwitched && (amountOfLight > (newThreshold + 100)) )   
  {
    delaySec(60 * 60);
    amountOfLight = averLight(15, 5);
//    if ( amountOfLight > 900 )
//    {
      Serial.print("switch the light off ");
      Serial.println(amountOfLight);
      switchTheLight( 0 );
//    }
  }
  Serial.print("loop - "); 
  Serial.println(amountOfLight);
  digitalWrite(switchSensorPin, LOW);
  timeFromWatering += 1;
  Serial.print("TIME FROM WATERING "); 
  Serial.println(timeFromWatering);  
  delaySec(60);
}
