#include <Servo.h>
#define servoPin 11
#define LED 12
#define buzzerPin 13
#define buttonPin 7

unsigned long previousTLED = millis();
unsigned long previousTServo = millis();
unsigned long previousTBuzzer = millis();
long timeIntervalLED = 100;
long timeIntervalServo = 5000;
long timeIntervalBuzzer = 1000;
int ledState = 0, servoState = 90, buzzerState = 0;
int stateDoor = 0, stateEB = 0;

Servo myservo;
String payload[2] = { "B2", "W1" };

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(buttonPin, INPUT);
  myservo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentTime = millis();
  boolean btnState = digitalRead(buttonPin);
  if (btnState == 1)
  {
    stateEB += 1;
  }
  if (stateEB == 2)
  {
    stateEB = 0;
  }
  if (payload[1] == "W1" && stateEB != 1) {
    // task 1
    if (stateDoor == 0) {
      if (currentTime - previousTServo > timeIntervalServo) {
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

    // task 2
    if (currentTime - previousTLED > timeIntervalLED) {
      previousTLED = currentTime;

      if (ledState == 1) {
        ledState = 0;
      } else {
        ledState = 1;
      }
      digitalWrite(LED, ledState);
    }
    
    // task 3
    if (currentTime - previousTBuzzer > timeIntervalBuzzer) {
      previousTBuzzer = currentTime;

      if (buzzerState == 1) {
        noTone(buzzerPin);
        buzzerState = 0;
      } else {
        for (int i = 1; i <= 1000; i++) {
          tone(buzzerPin, 1000);
        }
        buzzerState = 1;
      }
    }
  }
  if (stateEB == 1)
  {
    myservo.write(90);    
    digitalWrite(LED, LOW);
    noTone(buzzerPin);
  }
}