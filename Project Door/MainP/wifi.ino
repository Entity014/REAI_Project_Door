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
    dataArray[8] = 0x50;
    dataArray[9] = 0x31;
  }
  if (stat == "Off")
  {
    dataArray[8] = 0x50;
    dataArray[9] = 0x30;
  }
  if (stat == "EON")
  {
    dataArray[10] = 0x45;
    dataArray[11] = 0x31;
  }
  if (stat == "EOFF")
  {
    dataArray[10] = 0x45;
    dataArray[11] = 0x30;
  }
  if (stat == "MasterOpen")
  {
    dataArray[12] = 0x4D;
    dataArray[13] = 0x30;
  }
  if (stat == "SlaveOpen")
  {
    dataArray[12] = 0x4D;
    dataArray[13] = 0x31;
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