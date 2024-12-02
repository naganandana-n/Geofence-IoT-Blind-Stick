#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Pin Definitions
#define gpsRxPin D1
#define gpsTxPin D2
SoftwareSerial neo6m(gpsTxPin, gpsRxPin); // GPS on SoftwareSerial

TinyGPSPlus gps;

// WiFi Credentials
const char* ssid = "Naganandana";         // Replace with your WiFi SSID
const char* password = ""; // Replace with your WiFi password

// Server Configuration
const char* serverAddress = "http://192.168.7.132:3000/update-coords"; // Replace <server_ip> with your server's IP
// MAKE SURE NODEMCU AND LAPTOP CONNECTED TO THE SAME WIFI
// MAKE SURE TO CHANGE THIS IN SERVER.JS ALSO
// AND CHECK INDEX.HTML FILE ALSO (I DONT THINK ANY CHANGE IS THERE - BUT FUCKING CHECK IT)

void setup() {
  Serial.begin(115200); // Start Serial Monitor
  neo6m.begin(9600);    // Start GPS communication

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
}


void loop() {
  smartdelay_gps(1000); // Collect GPS data with a delay

  if (gps.location.isValid()) {
    double latitude = gps.location.lat();
    double longitude = gps.location.lng();

    // Send GPS coordinates to the server
    if (WiFi.status() == WL_CONNECTED) {
      sendGpsData(latitude, longitude);
    } else {
      Serial.println("WiFi not connected. Skipping data upload.");
    }

    // Debug Output to Serial Monitor
    Serial.print("Latitude: ");
    Serial.println(latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);
  } else {
    Serial.println("Waiting for valid GPS data...");
  }
}

void sendGpsData(double latitude, double longitude) {
  HTTPClient http;
  WiFiClient client;

  // Create JSON payload
  String payload = "{\"lat\":" + String(latitude, 6) + ",\"lng\":" + String(longitude, 6) + "}";

  // Send POST request to server
  http.begin(client, serverAddress);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    Serial.print("Server Response: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error Sending Data: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}

static void smartdelay_gps(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (neo6m.available())
      gps.encode(neo6m.read());
  } while (millis() - start < ms);
}