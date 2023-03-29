#include <Servo.h>
#define ledPin1 13
#define ledPin2 12
#define servoPin 11
#define buzzerPin 10

Servo myservo;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  myservo.attach(servoPin);
  myservo.write(90);
  tone(buzzerPin, 1000);
  delay(1000);
  noTone(buzzerPin);
  myservo.write(90-40);
  delay(1500);
  myservo.write(90);

}

void loop() {
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);

}
