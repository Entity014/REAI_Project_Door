#include <Wire.h>
#include <Servo.h>
#define slaveAddress 0x08
#define LED1 13
#define LED2 12
#define LED3 11
#define LED4 10

Servo myservo;

int stateDoor = 0, stateEB = 0;
byte dataArray[8];
String payload[(sizeof(dataArray)/sizeof(dataArray[0]))/2];

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  myservo.attach(9);
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
  if(stateEB == 0)
  {
    if(stateDoor == 0)
    {
      if(payload[0] == "B1" && payload[2] == "T0")
      {
        digitalWrite(LED1, HIGH);
        myservo.write(0);
        stateDoor = 1;
        delay(5000);
      }
      if(payload[0] == "B1" && payload[2] == "T1")
      {
        digitalWrite(LED1, HIGH);
        myservo.write(0);
        stateDoor = 1;
        delay(2500);
      }
      if(payload[3] == "W1")
      {
        myservo.write(0);
        stateDoor = 1;
        delay(5000);
      }
    }
    else if(stateDoor == 1)
    {
      if(payload[0] == "B1" && payload[2] == "T0")
      {
        digitalWrite(LED2, HIGH);
        myservo.write(180);
        stateDoor = 0;
        delay(5000);
      }
      if(payload[0] == "B1" && payload[2] == "T1")
      {
        digitalWrite(LED2, HIGH);
        myservo.write(180);
        stateDoor = 0;
        delay(2500);
      }
      if(payload[3] == "W1")
      {
        digitalWrite(LED4, HIGH);
        delay(100);
        digitalWrite(LED4, LOW);
        delay(100);
      }
    }
    if (payload[0] == "L1")
    {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      myservo.write(92);
    }
    if(payload[2] == "T0")
    {
      digitalWrite(LED3, LOW);
    }
    if (payload[2] == "T1")
    {
      digitalWrite(LED3, HIGH);
    }
    if (payload[1] == "B2")
    {
      stateEB = 1;
    }
  }
  else if(stateEB == 1)
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    myservo.write(92);
    if (payload[1] == "B2")
    {
      stateEB = 0;
    }
  }
}