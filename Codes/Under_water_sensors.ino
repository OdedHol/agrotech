#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <Adafruit_MLX90614.h>
#include <WiFi.h>
#include "ThingSpeak.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
WiFiClient client;

// Set the ThingSpeak channel and API key information
unsigned long myChannelNumber = 2184926;
const char* myWriteAPIKey = "JDZ7WLK4UNQE5CEW";

// Set the WiFi network credentials
const char* ssid = "TP-Link_905D";
const char* password = "33072036";

// Set the ThingSpeak server address
const char* server = "api.thingspeak.com";

// Set the time to wait between uploading data to ThingSpeak (in milliseconds)
int wait_between_uploads = 10000; // 10 seconds

void setup() {
  Serial.begin(115200);

  if (!sht31.begin(0x44)) {
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  mlx.begin();

  // Disconnect any previous WiFi connection
  WiFi.disconnect();
  delay(10);

  // Connect to the WiFi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("ESP32 connected to WiFi: ");
  Serial.println(ssid);
  Serial.println();

  // Initialize the ThingSpeak library with the WiFi client
  ThingSpeak.begin(client);
}

void loop() {
  // Read SHT31 sensor values
  float shtTemperature = sht31.readTemperature();
  float shtHumidity = sht31.readHumidity ();
  float mlxambient = mlx.readAmbientTempC();
  float mlxobject = mlx.readObjectTempC();

  if (!isnan(shtTemperature)) {
    Serial.print("SHT Temp *C = ");
    Serial.println(shtTemperature);
  } else {
    Serial.println("Failed to read SHT temperature");
  }

  if (!isnan(shtHumidity)) {
    Serial.print("SHT Hum. % = ");
    Serial.println(shtHumidity);
  } else {
    Serial.println("Failed to read SHT humidity");
  }

  // Read MLX90614 sensor values
  Serial.print("Ambient = ");
  Serial.print(mlx.readAmbientTempC());
  Serial.print("*C\tObject = ");
  Serial.print(mlx.readObjectTempC());
  Serial.println("*C");

  // Set the values to be sent to ThingSpeak
  ThingSpeak.setField(1, shtTemperature);
  ThingSpeak.setField(2, shtHumidity);
  ThingSpeak.setField(3, mlxambient);
  ThingSpeak.setField(4, mlxobject);

  // Send the data to ThingSpeak
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  // Print a message to the serial monitor indicating that the data has been uploaded
  Serial.println("Uploaded to ThingSpeak server.");

  // Disconnect the WiFi client
  client.stop();

  // Wait for the specified amount of time before uploading the next set of data
  Serial.println("Waiting to upload next reading...");
  Serial.println();

  delay(wait_between_uploads);
}
