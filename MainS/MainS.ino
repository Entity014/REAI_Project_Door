#include "Pin.h"
char payload[10];

void setup()
{
  Serial.begin(9600);
  buzzer(pinBuzzer);
  setupButton(pinLED, pinButton);
}

void loop()
{
  Button(pinLED, pinButton);
  Serial.readBytes(payload,5);
  Serial.println(payload);
  delay(1000);
}