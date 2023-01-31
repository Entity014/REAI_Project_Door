#include <Servo.h>
int LED1 = 13;
int LED2 = 12;
int LED3 = 11;
int LED4 = 10;
char message[10];
String payload[4];
String pay;
String command;

Servo myservo;

void setup()
{
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  myservo.attach(9);
}

void loop()
{
  if(Serial.available())
  {
    Serial.readBytes(message,6);
    pay = String(message);
    for(int i = 0; i < sizeof(payload)/sizeof(payload[0]); i++)
    {
      payload[i] = String(pay[i*2]) + String(pay[i*2+1]);
    }
    Serial.println(message);
    if (payload[0] == "B1" && payload[2] == "SU")
    {
      digitalWrite(LED1, HIGH);
      myservo.write(0);
      delay(5000);
    }
    else if (payload[0] == "B1" && payload[2] == "NI")
    {
      digitalWrite(LED2, HIGH);
      myservo.write(180);
      delay(2500);
    }
    else if (payload[0] == "LA")
    {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      myservo.write(90);
    }
    if(payload[1] == "WF")
    {
      digitalWrite(LED3, HIGH);
    }
    else if (payload[1] == "MF")
    {
      digitalWrite(LED3, LOW);
    }
    if(payload[2] == "NI")
    {
      digitalWrite(LED4, HIGH);
    }
    else if (payload[2] == "SU")
    {
      digitalWrite(LED4, LOW);
    }
  }
  delay(20); 
}