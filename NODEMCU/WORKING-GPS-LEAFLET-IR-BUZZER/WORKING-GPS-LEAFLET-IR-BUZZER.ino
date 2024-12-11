#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Pin Definitions
#define gpsRxPin D1           // GPS RX Pin
#define gpsTxPin D2           // GPS TX Pin
#define irSensorPin D3        // IR sensor pin
#define buzzerPin D6          // Passive buzzer pin

SoftwareSerial neo6m(gpsTxPin, gpsRxPin); // GPS on SoftwareSerial
TinyGPSPlus gps;

// WiFi Credentials
const char* ssid = "Naganandana";          // Replace with your WiFi SSID
const char* password = "";                // Replace with your WiFi password

// Server Configuration
const char* serverAddress = "http://3.28.218.118:3000/update-coords"; // Server Address

void setup() {
  Serial.begin(115200);
  neo6m.begin(9600);

  // Initialize IR Sensor and Buzzer
  pinMode(irSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check IR Sensor
  int irSensorValue = digitalRead(irSensorPin);
  if (irSensorValue == LOW) {   // LOW indicates ground detected
    Serial.println("IR Sensor: Ground Detected!");
    triggerBuzzer();            // Trigger the buzzer
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  // GPS Data Collection
  smartdelay_gps(1000);         // Delay and process GPS data

  if (gps.location.isValid()) {
    double latitude = gps.location.lat();
    double longitude = gps.location.lng();

    // Send GPS data
    if (WiFi.status() == WL_CONNECTED) {
      sendGpsData(latitude, longitude);
    }

    // Debug: Display GPS data
    Serial.print("Latitude: ");
    Serial.println(latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);
  } else {
    Serial.println("Waiting for valid GPS data...");
  }
}

// Function to trigger the buzzer manually at 1kHz
void triggerBuzzer() {
  Serial.println("Buzzer: Beeping...");
  for (int i = 0; i < 1000; i++) { 
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(500); // Half cycle
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(500);
  }
}

// Function to send GPS data to the server
void sendGpsData(double latitude, double longitude) {
  HTTPClient http;
  WiFiClient client;

  String payload = "{\"lat\":" + String(latitude, 6) + ",\"lng\":" + String(longitude, 6) + "}";
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

// GPS smart delay function
static void smartdelay_gps(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (neo6m.available()) gps.encode(neo6m.read());
  } while (millis() - start < ms);
}