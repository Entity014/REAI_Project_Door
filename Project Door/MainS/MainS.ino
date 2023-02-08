#include <Wire.h>
#include <Servo.h>
#define slaveAddress 0x08
#define ledPin1 13 // ไฟเวลา
#define ledPin2 12 // ไฟน้ำท่วม
#define servoPin 11
#define buzzerPin 10

Servo myservo;

unsigned long previousTLED1 = millis();
unsigned long previousTLED2 = millis();
unsigned long previousTServo = millis();
unsigned long previousTBuzzer = millis();
long timeIntervalLED = 100;
long timeIntervalServoD = 5000;
long timeIntervalServoN = 2500;
long timeIntervalBuzzer = 1000;
int ledState = 0, servoState = 90, stateDoor = 0, stateB = 0, buzzerState = 0;
byte dataArray[14];
String payload[(sizeof(dataArray)/sizeof(dataArray[0]))/2]; // [B1, B2, T0, W0, P0, E0, M0]

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

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  myservo.attach(servoPin);
  Wire.begin(slaveAddress); 
  Wire.onReceive(receiveEvent);
}

void loop()
{
  unsigned long currentTime = millis() + 5000;
  if ((payload[0] == "B1" || payload[4] == "P1") && (payload[1] != "B2" || payload[5] != "E1") && payload[2] == "T0")
  {
    if (stateDoor == 0)
    {
      if (currentTime - previousTServo > timeIntervalServoD)
      {
        previousTServo = currentTime;
        if (servoState == 0)
        {
          servoState = 90;
          stateDoor = 1;
        }
        else
        {
          servoState = 0;
        }
        myservo.write(servoState);
      }
    }
    else if (stateDoor == 1)
    {
      if (currentTime - previousTServo > timeIntervalServoD)
      {
        previousTServo = currentTime;
        if (servoState == 180)
        {
          servoState = 90;
          stateDoor = 0;
        }
        else
        {
          servoState = 180;
        }
        myservo.write(servoState);
      }
    }
  }
  if ((payload[0] == "B1" || payload[4] == "P1") && (payload[1] != "B2" || payload[5] != "E1") && payload[2] == "T1")
  {
    if (stateDoor == 0)
    {
      if (currentTime - previousTServo > timeIntervalServoN)
      {
        previousTServo = currentTime;
        if (servoState == 0)
        {
          servoState = 90;
          stateDoor = 1;
        }
        else
        {
          servoState = 0;
        }
        myservo.write(servoState);
      }
    }
    else if (stateDoor == 1)
    {
      if (currentTime - previousTServo > timeIntervalServoN)
      {
        previousTServo = currentTime;
        if (servoState == 180)
        {
          servoState = 90;
          stateDoor = 1;
        }
        else
        {
          servoState = 180;
        }
        myservo.write(servoState);
      }
    }
  }
  if ((payload[6] == "M0" || payload[6] == "M1") && (payload[1] != "B2" || payload[5] != "E1") && payload[2] == "T0")
  {
    if (stateDoor == 0)
    {
      if (currentTime - previousTServo > timeIntervalServoD)
      {
        previousTServo = currentTime;
        if (servoState == 0)
        {
          servoState = 90;
          stateDoor = 1;
        }
        else
        {
          servoState = 0;
        }
        myservo.write(servoState);
      }
    }
    else if (stateDoor == 1)
    {
      if (currentTime - previousTServo > timeIntervalServoD)
      {
        previousTServo = currentTime;
        if (servoState == 180)
        {
          servoState = 90;
          stateDoor = 1;
        }
        else
        {
          servoState = 180;
        }
        myservo.write(servoState);
      }
    }
  }
  if (payload[6] == "M0" && (payload[1] != "B2" || payload[5] != "E1") && payload[2] == "T1")
  {
    if (stateDoor == 0)
    {
      if (currentTime - previousTServo > timeIntervalServoD)
      {
        previousTServo = currentTime;
        if (servoState == 0)
        {
          servoState = 90;
          stateDoor = 1;
        }
        else
        {
          servoState = 0;
        }
        myservo.write(servoState);
      }
    }
    else if (stateDoor == 1)
    {
      if (currentTime - previousTServo > timeIntervalServoD)
      {
        previousTServo = currentTime;
        if (servoState == 180)
        {
          servoState = 90;
          stateDoor = 1;
        }
        else
        {
          servoState = 180;
        }
        myservo.write(servoState);
      }
    }
  }
  if (payload[6] == "M1" && (payload[1] != "B2" || payload[5] != "E1") && payload[2] == "T1")
  {
    if (stateDoor == 0)
    {
      if (currentTime - previousTServo > timeIntervalServoN)
      {
        previousTServo = currentTime;
        if (servoState == 0)
        {
          servoState = 90;
          stateDoor = 1;
        }
        else
        {
          servoState = 0;
        }
        myservo.write(servoState);
      }
    }
    else if (stateDoor == 1)
    {
      if (currentTime - previousTServo > timeIntervalServoN)
      {
        previousTServo = currentTime;
        if (servoState == 180)
        {
          servoState = 90;
          stateDoor = 1;
        }
        else
        {
          servoState = 180;
        }
        myservo.write(servoState);
      }
    }
  }
  if (payload[3] == "W1" && (payload[1] != "B2" || payload[5] != "E1"))
  {
    WaterFlood(currentTime);
  }
  if (payload[2] == "T1")
  {
    digitalWrite(ledPin2, HIGH);
  }
  if (payload[1] == "B2" || payload[5] == "E1")
  {
    myservo.write(90);
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    noTone(buzzerPin);
  }
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
}
