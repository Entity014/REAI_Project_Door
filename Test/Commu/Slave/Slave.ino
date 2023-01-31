#include <Wire.h>
#define slaveAddress 0x08
byte dataArray[8];
String payload[(sizeof(dataArray)/sizeof(dataArray[0]))/2];
void setup() {
  Serial.begin(9600);
  // Start the I2C Bus as Slave on address 9
  Wire.begin(slaveAddress); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
}
void receiveEvent(int bytes) {
  for(int i=0; i < bytes; i++)
  {
    dataArray[i] = Wire.read();
  }
  for(int i = 0; i < (sizeof(dataArray)/sizeof(dataArray[0]))/2; i++)
  {
    payload[i] = String(char(dataArray[2*i])) + String(char(dataArray[2*i+1]));
  }
}
void loop() {
  for(int i = 0; i < (sizeof(payload)/sizeof(payload[0])); i++)
  {
    Serial.println(payload[i]);
  }
}