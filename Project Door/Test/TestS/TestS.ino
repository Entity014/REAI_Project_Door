int LED1 = 13;
int LED2 = 12;
char message;

void setup()
{
  Serial.begin(9600); // communication
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop()
{
  if(Serial.available())
  {
    message = Serial.read();
    if(message == '1')
    {
      digitalWrite(LED1, 1);
    }
    else if(message == '2')
    {
      digitalWrite(LED2, 1);
    }
  }
  delay(20); 
  digitalWrite(LED1, 0);
  digitalWrite(LED2, 0);
}