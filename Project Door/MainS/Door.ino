void door(unsigned long now, long delay)
{
  if (stateDoor == 0)
    {
      if (now - previousTServo > delay)
      {
        previousTServo = now;
        if (servoState == 0)
        {
          servoState = 90;
          stateDoor = 1;
        }
        else
        {
          servoState = 0;
        }
        myservo.write(servoState);
      }
    }
    else if (stateDoor == 1)
    {
      if (now - previousTServo > delay)
      {
        previousTServo = now;
        if (servoState == 180)
        {
          servoState = 90;
          stateDoor = 0;
        }
        else
        {
          servoState = 180;
        }
        myservo.write(servoState);
      }
    }
}