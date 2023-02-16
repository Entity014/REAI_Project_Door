void statePayload(int &stateB1, int &stateB2, int &stateP1, int &stateF)
{
  if (payload[0] == "B1" || payload[4] == "P1" || payload[6] == "M2")
  {
    stateB1++;
  }
  if (payload[6] == "M1")
  {
    stateP1++;
  }
  if (payload[1] == "B2" || payload[5] == "E1")
  {
    stateB2++;
  }
  if (payload[3] == "W1")
  {
    stateF++;
  }
}

void door(unsigned long now, long delay, int &stateB1) {
  if (stateDoor == 0) {
    if (now - previousTServo > delay) {
      previousTServo = now;
      if (servoState == 0) {
        servoState = 90;
        stateDoor = 1;
        stateB1 = 0;
      } else {
        servoState = 0;
      }
      myservo.write(servoState);
    }
  } else if (stateDoor == 1) {
    if (now - previousTServo > delay) {
      previousTServo = now;
      if (servoState == 180) {
        servoState = 90;
        stateDoor = 0;
        stateB1 = 0;
      } else {
        servoState = 180;
      }
      myservo.write(servoState);
    }
  }
}