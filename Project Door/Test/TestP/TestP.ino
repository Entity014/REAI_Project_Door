int pButton1 = 9;
int pButton2 = 10;


void setup() {
  Serial.begin(9600);
  pinMode(pButton1, INPUT);
  pinMode(pButton2, INPUT);

}

void loop() {
  boolean btnState1 = digitalRead(pButton1);
  boolean btnState2 = digitalRead(pButton2);
  if (btnState1 == HIGH) {
    Serial.write('1');
  }
  else if (btnState2 == HIGH)
  {
    Serial.write('2');
  }
  delay(20);
}