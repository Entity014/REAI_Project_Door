#include <Wire.h>
#define slaveAddress 0x08
byte dataArray[8] = {0x42, 0x31, 0x42, 0x32, 0x54, 0x30, 0x57, 0x30};

void setup() {
  Wire.begin();
  Wire.beginTransmission(slaveAddress); //address is queued for checking if the slave is present
  for (int i=0; i < sizeof(dataArray)/sizeof(dataArray[0]); i++)
  {
    Wire.write(dataArray[i]);
  }
  Wire.endTransmission();
}

void loop() {
  // Wire.beginTransmission(9); // transmit to device #9
  // Wire.write("A");              // sends x 
  // Wire.endTransmission();    // stop transmitting
  // Serial.println(x);
  // x++; // Increment x
  // if (x > 10) x = 0; // `reset x once it gets 6
  // delay(500);
}