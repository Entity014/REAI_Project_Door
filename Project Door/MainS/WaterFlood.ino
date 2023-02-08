void WaterFlood(unsigned long currentTime) {
  if (currentTime - previousTLED2 > timeIntervalLED) {
    previousTLED2 = currentTime;

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
        stateB = 1;
      } else {
        servoState = 0;
      }
      myservo.write(servoState);
    }
  }

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
