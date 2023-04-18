#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

#define slaveAddress 0x50
#define buttonPin1 14
#define buttonPin2 12 
#define tricPin 17
#define echoPin 16
#define analogPin 34

// WIFI
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWD;
const char* mqtt_server = MQTT_SERVER;
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];

byte dataArray[14]; // [B1, B2, T0, W0, P0, E0, M0]
int light = 0, buttonState1 = 0, stateDoor = 0;
long duration, distance;
boolean btnState1, btnState2;

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(tricPin, OUTPUT);
  pinMode(echoPin, INPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{
  // Sensor
  Button(btnState1, btnState2);
  LDR();
  Ping();

  // MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    char out[128];
    char payload1[100];
    char payload2[100];
    char payload3[100];
    char temp3[100];
    StaticJsonDocument<256> doc;
    doc["LDR"] = light;
    doc["Ultrasonic"] = distance;
    serializeJson(doc, out);
    sprintf(payload1, out);
    sprintf(payload2, ((String)btnState1).c_str());
    sprintf(payload3, ((String)btnState2).c_str());
    sprintf(temp3, ((String)btnState2).c_str());
    if (buttonState1 >= 1)
    {
      if (stateDoor == 0)
      {
        client.publish("reai/door/b1", "1");
        buttonState1 = 0;
        stateDoor = 1;
        delay(500);
      }
      else if (stateDoor == 1)
      {
        client.publish("reai/door/b1", "0");
        buttonState1 = 0;
        stateDoor = 0;
        delay(500);
      }
    }
    if (temp3 != payload3)
    {
      client.publish("reai/door/b2", payload3);
    }
    // Serial.print("Publish message: ");
    // Serial.print(payload1);
    // Serial.print(", ");
    // Serial.print(payload2);
    // Serial.print(", ");
    // Serial.println(payload3);
    client.publish("reai/sensor", payload1);
  }

  // Send to arduino
  Wire.beginTransmission(slaveAddress); //address is queued for checking if the slave is present
  for (int i=0; i < sizeof(dataArray)/sizeof(dataArray[0]); i++)
  {
    Wire.write(dataArray[i]);
    // Serial.println((char)dataArray[i]);
  }
  Wire.endTransmission();
}