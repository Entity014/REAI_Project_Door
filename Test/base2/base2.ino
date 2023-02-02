#include <Servo.h>
#define ledPin2 10
#define ledPin1 11
#define servoPin 12
#define buttonPin1 13
#define buttonPin2 7
#define tricPin 9
#define echoPin 8

Servo myservo;
long duration, distance;
unsigned long previousTLED = millis();
unsigned long previousTServo = millis();
long timeIntervalLED = 100;
long timeIntervalServoD = 5000;
int ledState = 0, servoState = 90, stateDoor = 0;

void WaterFlood(unsigned long currentTime) {
  if (currentTime - previousTLED > timeIntervalLED) {
    previousTLED = currentTime;

    if (ledState == 1) {
      ledState = 0;
    } else {
      ledState = 1;
    }
    digitalWrite(ledPin1, ledState);
  }
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
}

void Ping(unsigned long currentTime) {
  digitalWrite(tricPin, LOW);
  delayMicroseconds(5);
  digitalWrite(tricPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(tricPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  if (distance < 12) {
    WaterFlood(currentTime);
  }
  delay(100);
}

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(tricPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(servoPin);
}
void loop() {
  unsigned long currentTime = millis() + 5000;
  boolean btnState1 = digitalRead(buttonPin1);
  boolean btnState2 = digitalRead(buttonPin2);
  Serial.println(btnState2);
  Ping(currentTime);

  if (btnState1 == 1 && btnState2 != 1) {
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
    } else if (stateDoor == 1) {
      digitalWrite(ledPin2, HIGH);
      myservo.write(180);
      delay(5000);
      myservo.write(90);
      stateDoor = 0;
    }
  }
  if (btnState2 == 1) {
    myservo.write(90);
  }
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
}
