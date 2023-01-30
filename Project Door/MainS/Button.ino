void setupButton(int pLED, int pButton)
{
  pinMode(pLED, OUTPUT);
  pinMode(pButton, INPUT);
}

void  Button(int pLED, int pButton)
{
  boolean btnState = digitalRead(pButton);

  if (btnState == HIGH) {
    digitalWrite(pLED, HIGH);
  } else {
    digitalWrite(pLED, LOW);
  }
}