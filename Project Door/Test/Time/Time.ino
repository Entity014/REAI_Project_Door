#include <Servo.h>
#define servoPin1 13
#define servoPin2 12

Servo servo1;
Servo servo2;

unsigned long previousTServo1 = millis();
unsigned long previousTServo2 = millis();
long timeIntervalServo1 = 5000;
long timeIntervalServo2 = 5000;
int servoState1 = 90, servoState2 = 90;
int stateDoor = 0, stateEB = 0;

String payload[2] = { "T0", "T1" };
void setup() {
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
}

void loop() {
  unsigned long currentTime = millis();
  if (stateDoor == 0) {
    if (payload[0] == "T0") {
      if (currentTime - previousTServo1 > timeIntervalServo1) {
        previousTServo1 = currentTime;

        if (servoState1 == 0) {
          servoState1 = 90;
          stateDoor = 1;
        } else {
          servoState1 = 0;
        }
        servo1.write(servoState1);
      }
    } else if (payload[0] == "T1") {
      if (currentTime - previousTServo2 > timeIntervalServo2) {
        previousTServo2 = currentTime;

        if (servoState2 == 0) {
          servoState2 = 90;
          stateDoor = 1;
        } else {
          servoState2 = 0;
        }
        servo2.write(servoState2);
      }
    }
  }
}