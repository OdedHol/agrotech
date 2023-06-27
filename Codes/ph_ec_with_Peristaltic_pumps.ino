//working
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h>

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const int relay_pin_1 = A4;
const int relay_pin_2 = D11;
String msgString;

const char* ssid = "TP-Link_905D";
const char* password = "33072036";
const char* mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7200;
const int daylightOffset_sec = 3600;

void callback(char* topic, byte* message, unsigned int length) {
  // Convert byte array to string
  String msgString = String((char*)message, length);

  // Convert string to integer
  int relayState = msgString.toInt();

  Serial.print("Message: ");
  Serial.println(msgString);
  Serial.print("Relay State: ");
  Serial.println(relayState);

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  if (strcmp(topic, "agrotech/2023/relay/1") == 0) {
    // If the relay is on, turn it off (and vice versa)
    if ((msgString == "1") && ((timeinfo.tm_hour == 6) || (timeinfo.tm_hour == 18)) && (timeinfo.tm_min < 30)) {
      digitalWrite(relay_pin_1, HIGH);
    }
    else {
      digitalWrite(relay_pin_1, LOW);
    }
  }

  if (strcmp(topic, "agrotech/2023/relay/2") == 0) {
    // If the relay is on, turn it off (and vice versa)
    if ((msgString == "1") && ((timeinfo.tm_hour == 7) || (timeinfo.tm_hour == 18)) && (timeinfo.tm_min < 30)) {
      digitalWrite(relay_pin_2, HIGH);
    }
    else {
      digitalWrite(relay_pin_2, LOW);
    }
  }
}

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
}

void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to Wi-Fi");
}

void setupTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void setup() {
  Serial.begin(115200);

  setupWiFi();
  setupTime();
  setupMQTT();

  pinMode(relay_pin_1, OUTPUT);
  pinMode(relay_pin_2, OUTPUT);

  digitalWrite(relay_pin_1, LOW); // Set relay 1 LOW
  digitalWrite(relay_pin_2, LOW); // Set relay 2 LOW
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }

  mqttClient.loop();
  long now = millis();
  printLocalTime();
  delay(1000);
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
    if (mqttClient.connect("AgrotechDevice")) {
      Serial.println("Connected to MQTT Broker");
      mqttClient.subscribe("agrotech/2023/relay/1");
      mqttClient.subscribe("agrotech/2023/relay/2");
    }
    else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  Serial.print("Current time: ");
  Serial.print(timeinfo.tm_hour);
  Serial.print(":");
  Serial.print(timeinfo.tm_min);
  Serial.print(":");
  Serial.println(timeinfo.tm_sec);
}
