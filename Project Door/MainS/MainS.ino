#include <Wire.h>
#include <Servo.h>
#define slaveAddress 0x50
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
long timeIntervalServoD = 1100;
long timeIntervalServoN = 700;
long timeIntervalBuzzer = 1000;
int ledState = 0, servoState = 90, stateDoor = 0, stateB = 0, buzzerState = 0;
int stateB1 = 0, stateB2 = 0, stateP1 = 0, stateF = 0;
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
  statePayload(stateB1, stateB2, stateP1, stateF);

  if (payload[2] == "T0")
  {
    digitalWrite(ledPin2, LOW);
    if (stateB2 == 0)
    {
      if (stateB1 >= 1 || stateP1 >= 1)
      {
        door(currentTime, timeIntervalServoD, stateB1);
        currentTime += 5000;
        stateP1 = 0;
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
        door(currentTime, timeIntervalServoN, stateB1);
        currentTime += 5000;
      }
      else if (stateP1 >= 1)
      {
        door(currentTime, timeIntervalServoD, stateB1);       
        currentTime += 5000; 
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
    myservo.write(90);
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    noTone(buzzerPin);
    stateB2 = 0;
  }
  for (int i = 0; i < sizeof(payload)/sizeof(payload[0]); i++)
  {
    Serial.print(payload[i]);
    Serial.print(" ");
  }
  Serial.print(stateB1);
  Serial.print(" ");
  Serial.print(stateB2);
  Serial.print(" ");
  Serial.print(stateP1);
  Serial.print(" ");
  Serial.print(stateF);
  Serial.println();
  delay(100);
}
