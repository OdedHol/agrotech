#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "ThingSpeak.h"

// WiFi
const char* ssid = "TP-Link_905D";
const char* password = "33072036";

// MQTT
const char* mqttServer = "broker.hivemq.com";
int mqttPort = 1883;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// ThingSpeak
unsigned long myChannelNumber = 2184926;
const char* myWriteAPIKey = "JDZ7WLK4UNQE5CEW";
const char* server = "api.thingspeak.com";
int wait_between_uploads = 10000;
WiFiClient client;

// PH
const int pHPin = A2;
const int pH_V = D13;
float ph;
float Value = 0;

// Temperature
const int SENSOR_PIN = A4;
const int SENSOR_V = D12;
float tempCelsius;
float tempFahrenheit;

// EC
int TDS_PIN = A1;
int TDS_V = D11;
int val;
float voltage;
int EC = 0;

OneWire oneWire(SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

long previous_time = 0;
String msgString;

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT Broker..");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Connected.");
    }
  }
}

void setup() {
  // WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to Wi-Fi");

  // ThingSpeak
  ThingSpeak.begin(client);

  // MQTT
  setupMQTT();

  // PH
  Serial.begin(115200);
  pinMode(pH_V, OUTPUT);
  digitalWrite(pH_V, LOW);
  delay(1000);

  // Temperature
  tempSensor.begin();
  pinMode(SENSOR_V, OUTPUT);
  digitalWrite(SENSOR_V, LOW);

  // EC
  pinMode(TDS_V, OUTPUT);
  digitalWrite(TDS_V, LOW);
}

void loop() {
  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
  long now = millis();

  if (now - previous_time > 1000) {
    previous_time = now;

    // PH
    digitalWrite(pH_V, HIGH);
    delay(1000);
    Value = analogRead(pHPin);
    Serial.print(Value);
    Serial.print(" | ");
    voltage = Value * (3.3 / 4095.0);
    ph = (3.3 * voltage);
    Serial.println(ph);
    digitalWrite(pH_V, LOW);

    // Temperature
    digitalWrite(SENSOR_V, HIGH);
    delay(1000);
    tempSensor.requestTemperatures();
    tempCelsius = tempSensor.getTempCByIndex(0);
    tempFahrenheit = tempCelsius * 9 / 5 + 32;
    Serial.print("Temperature: ");
    Serial.print(tempCelsius);
    Serial.print("°C");
    Serial.print("  ~  ");
    Serial.print(tempFahrenheit);
    Serial.println("°F");
    digitalWrite(pH_V, LOW);

    // EC
    digitalWrite(TDS_V, HIGH);
    delay(1000);
    val = analogRead(TDS_PIN);
    voltage = (3.3 / 4095) * val;
    EC = voltage * 1000;
    Serial.print("EC: ");
    Serial.print(EC);
    digitalWrite(TDS_V, LOW);

    delay(5000);

    // ThingSpeak
    ThingSpeak.setField(6, ph);
    ThingSpeak.setField(7, tempCelsius);
    ThingSpeak.setField(8, EC);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    Serial.println("Uploaded to ThingSpeak server.");
    client.stop();

    // MQTT
    char tempString[8];
    dtostrf(tempCelsius, 1, 2, tempString);
     if (ph<5.5) {
    mqttClient.publish("agrotech/2023/relay/1", "1");
     }
      if (ph>5.5) {
    mqttClient.publish("agrotech/2023/relay/1", "0");
     }
     if (EC<6) {
    mqttClient.publish("agrotech/2023/relay/2", "1");
  }
    if (EC>6) {
    mqttClient.publish("agrotech/2023/relay/2", "0");
  }
}
}
