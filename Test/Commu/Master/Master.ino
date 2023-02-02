#include <Wire.h>
#define slaveAddress 0x08
#define buttonPin1 13
#define buttonPin2 12
#define tricPin 11
#define echoPin 10
#define analogPin 0
byte dataArray[8]; // [B1, B2, T0, W0]

int light = 0;
long duration, distance;

void Button()
{
  boolean btnState1 = digitalRead(buttonPin1);
  boolean btnState2 = digitalRead(buttonPin2);
  
  if (btnState1 == HIGH)
  {
    dataArray[0] = 0x42;
    dataArray[1] = 0x31;
  }
  if (btnState1 == LOW)
  {
    dataArray[0] = 0x4C;
    dataArray[1] = 0x31;
  }
  if (btnState2 == HIGH)
  {
    dataArray[2] = 0x42;
    dataArray[3] = 0x32;
  }
  if (btnState2 == LOW)
  {
    dataArray[2] = 0x4C;
    dataArray[3] = 0x32;
  }
}

void LDR()
{
  light = analogRead(analogPin);
  if (light <= 100)
  {
    dataArray[4] = 0x54;
    dataArray[5] = 0x31;
  }
  else
  {
    dataArray[4] = 0x54;
    dataArray[5] = 0x30;
  }
}

void Ping()
{
  digitalWrite(tricPin, LOW); 
  delayMicroseconds(5); 
  digitalWrite(tricPin, HIGH); 
  delayMicroseconds(5); 
  digitalWrite(tricPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if (distance < 12)
  {
    dataArray[6] = 0x57;
    dataArray[7] = 0x31;
  }
  else
  {
    dataArray[6] = 0x57;
    dataArray[7] = 0x30;
  }
  delay(100);
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(tricPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  Button();
  LDR();
  Ping();
  Wire.beginTransmission(slaveAddress); //address is queued for checking if the slave is present
  for (int i=0; i < sizeof(dataArray)/sizeof(dataArray[0]); i++)
  {
    Wire.write(dataArray[i]);
  }
  Wire.endTransmission();
}
