void Button(boolean &btnState1, boolean &btnState2)
{
  btnState1 = digitalRead(buttonPin1);
  btnState2 = digitalRead(buttonPin2);
  // Serial.print("button 1: ");
  // Serial.print(btnState1);
  // Serial.print(", button 2: ");
  // Serial.println(btnState2);
  if (btnState1 == HIGH)
  {
    dataArray[0] = 0x42;
    dataArray[1] = 0x31;
    buttonState1++;
  }
  if (btnState1 == LOW)
  {
    dataArray[0] = 0x4C;
    dataArray[1] = 0x31;
  }
  if (btnState2 == HIGH)
  {
    dataArray[2] = 0x42;
    dataArray[3] = 0x32;
  }
  if (btnState2 == LOW)
  {
    dataArray[2] = 0x4C;
    dataArray[3] = 0x32;
  }
}

void LDR() 
{
  light = analogRead(analogPin);
  // Serial.print("Light : ");
  // Serial.print(light);
  if (light <= 300)
  {
    dataArray[4] = 0x54;
    dataArray[5] = 0x31;
  }
  else
  {
    dataArray[4] = 0x54;
    dataArray[5] = 0x30;
  }
}

void Ping()
{
  digitalWrite(tricPin, LOW); 
  delayMicroseconds(5); 
  digitalWrite(tricPin, HIGH); 
  delayMicroseconds(5); 
  digitalWrite(tricPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  // Serial.print(", Distance : ");
  // Serial.println(distance);
  if (distance < 12)
  {
    dataArray[6] = 0x57;
    dataArray[7] = 0x31;
  }
  else
  {
    dataArray[6] = 0x57;
    dataArray[7] = 0x30;
  }
  delay(100);
}