#define buttonPin1 14
#define buttonPin2 12
#define tricPin 7
#define echoPin 6
#define analogPin 34

long duration, distance;
boolean btnState1, btnState2;
int light = 0;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(tricPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  btnState1 = digitalRead(buttonPin1);
  btnState2 = digitalRead(buttonPin2);
  light = analogRead(analogPin);
  digitalWrite(tricPin, LOW); 
  delayMicroseconds(5); 
  digitalWrite(tricPin, HIGH); 
  delayMicroseconds(5); 
  digitalWrite(tricPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  delay(100);
  Serial.print("Button1 : ");
  Serial.print(btnState1);
  Serial.print(" Button2 : ");
  Serial.print(btnState2);
  Serial.print(" Distance : ");
  Serial.print(distance);
  Serial.print(" Light : ");
  Serial.println(light);
}
