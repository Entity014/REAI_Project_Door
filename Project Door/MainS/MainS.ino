#include <Wire.h>
#include <Servo.h>
#define slaveAddress 0x50
#define ledPin1 12 // ไฟเวลา
#define ledPin2 13 // ไฟน้ำท่วม
#define servoPin 11
#define buzzerPin 10

Servo myservo;

unsigned long previousTLED1 = millis();
unsigned long previousTLED2 = millis();
unsigned long previousTServo = millis();
unsigned long previousTBuzzer = millis();
unsigned long start = 0, stop = 0;
int deltaTimeD = 0, deltaTimeN = 0, deltaTime = 0;
long timeIntervalLED = 100;
long timeIntervalServoD = 1200;
long timeIntervalServoN = 900;
long timeIntervalBuzzer = 1000;
int ledState = 0, servoState = 90, stateDoor = 0, stateB = 0, buzzerState = 0;
int stateB1 = 0, stateB2 = 0, stateP1 = 0, stateF = 0;
int doorWaterFlood = 0, tempStart = 0, tempStop = 0, tempTime = 0;
int tempEmer = 0;
bool emergencyMai = false;
byte dataArray[14];
String payload[(sizeof(dataArray)/sizeof(dataArray[0]))/2]; // [B1, B2, T0, W0, P0, E0, M0]

void receiveEvent(int bytes) {
  for(int i=0; i < bytes; i++)
  {
    dataArray[i] = Wire.read();
  }
  delay(100);
  for(int i = 0; i < (sizeof(dataArray)/sizeof(dataArray[0]))/2; i++)
  {
    payload[i] = String(char(dataArray[2*i])) + String(char(dataArray[2*i+1]));
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  myservo.attach(servoPin);
  Wire.begin(slaveAddress); 
  Wire.onReceive(receiveEvent);
}

void loop()
{
  unsigned long currentTime = millis() + 5000;
  deltaTime = abs(stop - start);
  statePayload(stateB1, stateB2, stateP1, stateF);

  if ((myservo.read() == 60 || myservo.read() == 120) && tempStart == 0)
  {
    start = currentTime;
    tempStart = 1;
    tempStop = 0;
    tempEmer = 1;
  }
  if (myservo.read() == 90 && tempStop == 0)
  {
    stop = currentTime; 
    tempStart = 0;
    tempStop = 1;
    tempEmer = 0;
  }

  if (payload[2] == "T0")
  {
    digitalWrite(ledPin2, LOW);
    if (stateB2 == 0)
    {
      if ((stateB1 >= 1 || stateP1 >= 1) && !emergencyMai)
      {
        door(currentTime, timeIntervalServoD, stateB1, doorWaterFlood);
        currentTime += 5000;
        stateP1 = 0;
      }
      else if ((stateB1 >= 1 || stateP1 >= 1) && emergencyMai)
      {
        door(currentTime, tempTime, stateB1, doorWaterFlood);
        currentTime += 5000;
        stateP1 = 0;
        if (myservo.read() == 90 && tempEmer == 1)
        {
          emergencyMai = false;
        }
      }
    }
  }
  if (payload[2] == "T1")
  {
    digitalWrite(ledPin2, HIGH);
    if (stateB2 == 0)
    {
      if (stateB1 >= 1)
      {
        if (doorWaterFlood == 1)
        {
          door(currentTime, timeIntervalServoD, stateB1, doorWaterFlood);
          currentTime += 5000;
        }
        else if (doorWaterFlood == 0 && !emergencyMai)
        {
          door(currentTime, timeIntervalServoN, stateB1, doorWaterFlood);
          currentTime += 5000; 
        }
        else if (doorWaterFlood == 0 && emergencyMai)
        {
          door(currentTime, tempTime, stateB1, doorWaterFlood);
          currentTime += 5000; 
          if (myservo.read() == 90 && tempEmer == 1)
          {
            emergencyMai = false;
          }
        }
      }
      else if (stateP1 >= 1 && !emergencyMai)
      {
        door(currentTime, timeIntervalServoD, stateP1, doorWaterFlood);
        currentTime += 5000;
      }
      else if (stateP1 >= 1 && emergencyMai)
      {
        door(currentTime, tempTime, stateP1, doorWaterFlood);
        currentTime += 5000;
        if (myservo.read() == 90 && tempEmer == 1)
        {
          emergencyMai = false;
        }
      }
    }
  }
  if (stateF >= 1)
  {
    WaterFlood(currentTime, stateF);
    currentTime += 5000;
  }
  if (payload[3] == "W0" && stateF >= 0)
  {
    digitalWrite(ledPin1, LOW);
    noTone(buzzerPin);
    stateF = 0;
  }
  if (stateB2 >= 1)
  {
    tempTime = deltaTime;
    myservo.write(90);
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    noTone(buzzerPin);
    stateB2 = 0;
    emergencyMai = true;
  }
  for (int i = 0; i < sizeof(payload)/sizeof(payload[0]); i++)
  {
    Serial.print(payload[i]);
    Serial.print(" ");
  }
  Serial.print(deltaTime);
  Serial.print(" ");
  Serial.print(emergencyMai);
  Serial.print(" ");
  Serial.print(tempEmer);
  Serial.println();
  delay(100);
}
