#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#define slaveAddress 0x08

#include "config.h"

byte dataArray[8]; // [B1, B2, T0, W0]

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWD;
const char* mqtt_server = MQTT_SERVER;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
long randNumber1;
long randNumber2;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String stat;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    stat += (char)payload[i];
  }
  Serial.println();

  if (stat == "On")
  {
    dataArray[0] = 0x42;
    dataArray[1] = 0x31;
  }
  else
  {
    dataArray[0] = 0x4C;
    dataArray[1] = 0x31;
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      char subscribe_topic[100];
      sprintf(subscribe_topic, "%s/door/#", MQTT_TOPIC_PREFIX);
      client.subscribe(subscribe_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  randomSeed(analogRead(0));
}

void loop() {
  randNumber1 = random(1000);
  randNumber2 = random(100);
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  Wire.beginTransmission(slaveAddress); //address is queued for checking if the slave is present
  for (int i=0; i < sizeof(dataArray)/sizeof(dataArray[0]); i++)
  {
    Wire.write(dataArray[i]);
  }
  Wire.endTransmission();
  
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    char out[128];
    char payload[100];
    StaticJsonDocument<256> doc;
    doc["LDR"] = randNumber1;
    doc["Ultrasonic"] = randNumber2;
    serializeJson(doc, out);
    sprintf(payload, out);
    // Serial.print("Publish message: ");
    // Serial.println(payload);
    client.publish("reai/sensor", payload);
  }
}
