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
  if (stat == "Off")
  {
    dataArray[0] = 0x4C;
    dataArray[1] = 0x31;
  }
  if (stat == "EON")
  {
    dataArray[2] = 0x42;
    dataArray[3] = 0x32;
  }
  if (stat == "EOFF")
  {
    dataArray[2] = 0x4C;
    dataArray[3] = 0x32;
  }
  if (stat == "MasterOpen")
  {
    dataArray[8] = 0x4D;
    dataArray[9] = 0x30;
  }
  if (stat == "SlaveOpen")
  {
    dataArray[8] = 0x4D;
    dataArray[9] = 0x31;
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