#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// GPS Pin Definitions
#define gpsRxPin D1
#define gpsTxPin D2
SoftwareSerial neo6m(gpsTxPin, gpsRxPin); // GPS on SoftwareSerial

TinyGPSPlus gps;

// IR Sensor and Buzzer Pin Definitions
#define irSensorPin D3  // IR Sensor connected to GPIO D3
#define buzzerPin D6    // Passive Buzzer connected to GPIO D6

// WiFi Credentials
const char* ssid = "Naganandana";         // Replace with your WiFi SSID
const char* password = "";                // Replace with your WiFi password

// Server Configuration
const char* serverAddress = "http://3.28.218.118:3000/update-coords"; // Replace <server_ip> with your server's IP
// MAKE SURE NODEMCU AND LAPTOP CONNECTED TO THE SAME WIFI
// MAKE SURE TO CHANGE THIS IN SERVER.JS ALSO
// AND CHECK INDEX.HTML FILE ALSO (I DONT THINK ANY CHANGE IS THERE - BUT FUCKING CHECK IT)

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);       // Start Serial Monitor
  neo6m.begin(9600);        // Start GPS communication

  // IR Sensor and Buzzer Setup
  pinMode(irSensorPin, INPUT);   // Set IR sensor pin as INPUT
  pinMode(buzzerPin, OUTPUT);    // Set buzzer pin as OUTPUT
  digitalWrite(buzzerPin, LOW);  // Ensure buzzer is off initially

  Serial.println("IR Sensor and GPS Test Starting...");

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
  // Handle GPS Data
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

  // Handle IR Sensor and Buzzer
  int irSensorValue = digitalRead(irSensorPin); // Read IR sensor value
  if (irSensorValue == LOW) {
    Serial.println("IR Sensor: Ground Detected! Beeping...");
    triggerBuzzer(); // Activate buzzer continuously
  } else {
    Serial.println("IR Sensor: No Ground Detected");
    digitalWrite(buzzerPin, LOW); // Ensure buzzer is off
  }
}

// Function to send GPS data to the server
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

// Function to process GPS data with a delay
static void smartdelay_gps(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (neo6m.available())
      gps.encode(neo6m.read());
  } while (millis() - start < ms);
}

// Function to trigger the buzzer continuously while the IR sensor detects ground
void triggerBuzzer() {
  // TEST AFTER THE DEMONSTRATION: 
  for (int i = 0; i < 500; i++) { 
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(125); // Half cycle of 1kHz (500µs HIGH)
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(125); // Half cycle of 1kHz (500µs LOW)
  }
}