int pButton1 = 9;
int pButton2 = 10;
int analogPin = 0;
int tricPin = 6;
int echoPin = 7;

int light = 0;
long duration, distance;

void Ping()
{
  String flood;
  digitalWrite(tricPin, LOW); 
  delayMicroseconds(5); 
  digitalWrite(tricPin, HIGH); 
  delayMicroseconds(5); 
  digitalWrite(tricPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if (distance < 12) Serial.write("WF", 2);
  else Serial.write("MF", 2);
  delay(100);
}

void Button()
{
  boolean btnState1 = digitalRead(pButton1);
  boolean btnState2 = digitalRead(pButton2);
  
  if (btnState1 == HIGH)
  {
    Serial.write("B1", 2);
  }
  else if (btnState2 == HIGH)
  {
    Serial.write("B2", 2);
  }
  else if (btnState1 == LOW && btnState2 == LOW)
  {
    Serial.write("LA", 2);
  }
}

void LDR()
{
  light = analogRead(analogPin);
  if (light <= 100) Serial.write("NI", 2);
  else Serial.write("SU", 2);
}

void setup() {
  Serial.begin(9600);
  pinMode(pButton1, INPUT);
  pinMode(pButton2, INPUT);
  pinMode(tricPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  Button();
  Ping();
  LDR();
  // Serial.print(distance); 
  // Serial.print(" cm");
  // Serial.print(", LDR : ");
  // Serial.println(light);
}