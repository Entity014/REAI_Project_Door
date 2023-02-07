#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

#define slaveAddress 0x08
#define buttonPin1 13
#define buttonPin2 12
#define tricPin 11
#define echoPin 10
#define analogPin 0

// WIFI
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWD;
const char* mqtt_server = MQTT_SERVER;
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];

byte dataArray[10]; // [B1, B2, T0, W0, M0]
int light = 0;
long duration, distance;

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
  Button();
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
    char payload[100];
    StaticJsonDocument<256> doc;
    doc["LDR"] = light;
    doc["Ultrasonic"] = distance;
    serializeJson(doc, out);
    sprintf(payload, out);
    Serial.print("Publish message: ");
    Serial.println(payload);
    client.publish("reai/sensor", payload);
  }

  // Send to arduino
  Wire.beginTransmission(slaveAddress); //address is queued for checking if the slave is present
  for (int i=0; i < sizeof(dataArray)/sizeof(dataArray[0]); i++)
  {
    Wire.write(dataArray[i]);
  }
  Wire.endTransmission();
}