#include <Wire.h>
#include <Servo.h>
#define slaveAddress 0x08
#define ledPin1 13
#define ledPin2 12
#define ledPin3 11
#define ledPin4 10
#define servoPin 9
#define buzzerPin 8

Servo myservo;

byte dataArray[8];
String payload[(sizeof(dataArray)/sizeof(dataArray[0]))/2]; // [B1, B2, T0, W0]
unsigned long previousTLED = millis();
unsigned long previousTServo = millis();
unsigned long previousTBuzzer = millis();
long timeIntervalLED = 100;
long timeIntervalServoD = 5000;
long timeIntervalServoN = 2500;
long timeIntervalBuzzer = 1000;
int servoState = 90, ledState = 0, buzzerState = 0;
int stateDoor = 0, stateEB = 0, stateB = 0;

void Door(unsigned long cT, long dT,int state, int start = 0, int stop = 90) {
  if (cT - previousTServo > dT) {
    previousTServo = cT;

    if (servoState == start) {

      stateDoor = state;
      servoState = stop;
    } else {
      servoState = start;
    }
    myservo.write(servoState);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  myservo.attach(servoPin);
  Wire.begin(slaveAddress); 
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
  for(int i=0; i < bytes; i++)
  {
    dataArray[i] = Wire.read();
  }
  for(int i = 0; i < (sizeof(dataArray)/sizeof(dataArray[0]))/2; i++)
  {
    payload[i] = String(char(dataArray[2*i])) + String(char(dataArray[2*i+1]));
  }
}

void loop() {
  unsigned long currentTime = millis();
  if (payload[0] == "B1")
  {
    if (stateB == 0)
    {
      stateB = 1;
    }
    else if (stateB == 1)
    {
      stateB = 2;
    }
    else if (stateB == 2)
    {
      stateB = 0;
    }
  }
  if (payload[1] == "B2")
  {
    if (stateEB == 0)
    {
      stateEB = 1;
    }
    else if (stateEB == 1)
    {
      stateEB = 0;
    }
  }
  if (payload[2] == "T1" && stateEB != 1)
  {
    digitalWrite(ledPin3, HIGH);
  }
  if (stateDoor == 0 && stateB == 1 && stateEB != 1) {
    if (payload[2] == "T0") {
      digitalWrite(ledPin1, HIGH);
      // Door(currentTime, timeIntervalServoD, 1);
      myservo.write(0);
      delay(timeIntervalServoD);
      stateDoor = 1;
      myservo.write(90);
    } else if (payload[2] == "T1") {
      digitalWrite(ledPin1, HIGH);
      // Door(currentTime, timeIntervalServoN, 1);
      myservo.write(0);
      delay(timeIntervalServoN);
      stateDoor = 1;
      myservo.write(90);
    }
  }
  if (stateDoor == 1 && stateB == 2 && stateEB != 1)
  {
    if (payload[2] == "T0") {
      digitalWrite(ledPin2, HIGH);
      myservo.write(180);
      delay(timeIntervalServoD);
      stateDoor = 0;
      myservo.write(90);
    } else if (payload[2] == "T1") {
      digitalWrite(ledPin2, HIGH);
      myservo.write(180);
      delay(timeIntervalServoN);
      stateDoor = 0;
      myservo.write(90);
    }
  }
  if (payload[3] == "W1" && stateEB != 1) {
    // task 1
    if (stateDoor == 0) {
      if (currentTime - previousTServo > timeIntervalServoD) {
        previousTServo = currentTime;

        if (servoState == 0) {
          servoState = 90;
          stateDoor = 1;
        } else {
          servoState = 0;
        }
        myservo.write(servoState);
      }
    }

    // task 2
    if (currentTime - previousTLED > timeIntervalLED) {
      previousTLED = currentTime;

      if (ledState == 1) {
        ledState = 0;
      } else {
        ledState = 1;
      }
      digitalWrite(ledPin4, ledState);
    }
    
    // task 3
    if (currentTime - previousTBuzzer > timeIntervalBuzzer) {
      previousTBuzzer = currentTime;

      if (buzzerState == 1) {
        noTone(buzzerPin);
        buzzerState = 0;
      } else {
        for (int i = 1; i <= 1000; i++) {
          tone(buzzerPin, 1000);
        }
        buzzerState = 1;
      }
    }
  }
  if (stateEB == 1)
  {
    myservo.write(90);
    noTone(buzzerPin);
  }
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
}