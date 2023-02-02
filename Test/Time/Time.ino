#include <Servo.h>
#define ledPin1 12
#define ledPin2 11
#define servoPin 13

Servo servo;

unsigned long previousTServo = millis();
long timeIntervalServoD = 5000;
long timeIntervalServoN = 2500;
int servoState = 90;
int stateDoor = 0, stateEB = 0;

String payload[2] = { "T0", "T1" };

void Door(unsigned long cT, long dT,int state, int start = 0, int stop = 90) {
  if (cT - previousTServo > dT) {
    previousTServo = cT;

    if (servoState == start) {

      stateDoor = state;
      servoState = stop;
    } else {
      servoState = start;
    }
    servo.write(servoState);
  }
}

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  servo.attach(servoPin);
}

void loop() {
  unsigned long currentTime = millis();
  if (stateDoor == 0) {
    if (payload[0] == "T0") {
      digitalWrite(ledPin1, HIGH);
      // task 1
      Door(currentTime, timeIntervalServoD, 1);
      // task 2
      digitalWrite(ledPin1, LOW);
    } else if (payload[0] == "T1") {
      digitalWrite(ledPin2, HIGH);
      // task 1
      Door(currentTime, timeIntervalServoN, 1);
      // task 2
      digitalWrite(ledPin2, LOW);
    }
  }
  if (stateDoor == 1)
  {
    if (payload[0] == "T0") {
      digitalWrite(ledPin2, HIGH);
      // task 1
      Door(currentTime, timeIntervalServoD, 0, 180);
      // task 2
      digitalWrite(ledPin2, LOW);
    } else if (payload[0] == "T1") {
      digitalWrite(ledPin1, HIGH);
      // task 1
      Door(currentTime, timeIntervalServoN, 0, 180);
      // task 2
      digitalWrite(ledPin1, LOW);
    }
  }
}