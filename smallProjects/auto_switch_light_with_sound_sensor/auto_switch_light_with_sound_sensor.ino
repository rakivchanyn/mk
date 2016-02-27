int analogPin = 0;     // potentiometer wiper (middle terminal) connected to analog pin 3

                       // outside leads to ground and +5V

//int val = 0;           // variable to store the value read
bool switchOn = false;
bool enableSoundSensor = false;
bool firstTimeSwitch = true;
volatile int i = 0;
volatile int val = 1000;

volatile int counter = 0;

void setup()

{
  Serial.begin(9600);          //  setup serial
  pinMode(4, OUTPUT);
  
  pinMode(5, OUTPUT);
  
  pinMode(6, INPUT);

  pinMode(9, INPUT);
}



void loop()

{
  i++;
  if (i > 100)
  {
    val = analogRead(analogPin);    // read the input pin
    i = 0;
  }
  //volatile int sound = analogRead(3);
  enableSoundSensor = (val < 300);
  if (enableSoundSensor && firstTimeSwitch)
  {
      firstTimeSwitch = false;
      switchOn = true;
  }
  else if ( !enableSoundSensor )
  {
      switchOn = false;
      firstTimeSwitch = true;;
  }
//  Serial.println(/*switchOn*/i);
//  Serial.println(val); 
  if (switchOn)
  {
      digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else if ( !switchOn || !enableSoundSensor )
  {
      digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(5, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
  }

//  Serial.println(enableSoundSensor);             // debug value
  volatile int state = digitalRead(9);

  if ( enableSoundSensor && state == 0)
  {
      counter++;
      Serial.print(counter);
      Serial.println(" - sound");
      if (switchOn)
        switchOn = false;
      else
        switchOn = true;

      delay(100);
  }
//  delay(500);
}
