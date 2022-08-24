#define PULSE_AVG_CNT 16
#define CAL_OFFSET 10
#define MODE_NONE 0
#define MODE_LED 1
#define MODE_BUZZER 2
#define MODE_BOTH 3

int rcPin = 0;     // PWM signal in from RC receiver - connect to Pin 5 (see above)
int buzzerPin = 2;  //Buzzer pin out to buzzer  - connect to Pin 6 (see above)
int ledPin = 1;     //Optional - connect to Pin 6 (see above)
int chPWMVal = 0;        // Receiver channel pwm value - connect to a free channel on your RX. Leave value at 0.
byte chMode = 0;

int pwmVals[PULSE_AVG_CNT];
int pwmValsCurrIx = 0;

void setup() {
  pinMode(rcPin, INPUT);
  pinMode(buzzerPin, OUTPUT);  
  pinMode(ledPin, OUTPUT);
}

void loop() {
  pwmVals[pwmValsCurrIx] = pulseIn(rcPin, HIGH, 25000);
  pwmValsCurrIx++;
  if(pwmValsCurrIx >= PULSE_AVG_CNT)
    pwmValsCurrIx = 0;

  chPWMVal = 0;
  for(byte i = 0; i < PULSE_AVG_CNT; i++)
  {
    chPWMVal += pwmVals[i];
  }
  chPWMVal = (chPWMVal / PULSE_AVG_CNT) + CAL_OFFSET;
  
  if(chPWMVal > 1000 && chPWMVal < 1120)
  {
    chMode = MODE_LED;
    chPWMVal = chPWMVal - 1000;
  }
  else if(chPWMVal > 1200 && chPWMVal < 1320)
  {
    chMode = MODE_BOTH;
    chPWMVal = chPWMVal - 1200;
  }
  else if(chPWMVal > 1400 && chPWMVal < 1520)
  {
    chMode = MODE_BUZZER;
    chPWMVal = chPWMVal - 1400;
  }
  else if(chPWMVal < 200) {
    chMode = MODE_BOTH;
    chPWMVal = 80;
  }
  else
    chMode = MODE_NONE;

  if(chPWMVal > 80)
    chPWMVal = 80;

  if((chMode | MODE_LED) > 0) {
    analogWrite(ledPin, map(chPWMVal, 0, 80, 0, 255));
  }
  else
    digitalWrite(ledPin, HIGH);
}
