#include <SoftwareSerial.h>

#define RX_PIN 4 // NodeMCU GPIO4 (D2)
#define TX_PIN 5 // NodeMCU GPIO5 (D1)

SoftwareSerial gpsSerial(RX_PIN, TX_PIN);

// Declare functions
void extractLatitudeLongitude(const String &nmea);
void splitNMEA(const String &nmea, String *fields, int maxFields);

void setup() {
  Serial.begin(115200);    // Debugging serial
  gpsSerial.begin(9600);   // GPS module baud rate
  Serial.println("GPS Module Test Starting...");
  delay(1000); // Wait for the GPS module to initialize
}

void loop() {
  // Check if data is available from GPS
  if (gpsSerial.available() > 0) {
    Serial.println("Data available from GPS module...");
    String nmeaSentence = ""; // Buffer to store NMEA sentence

    // Read a complete sentence
    while (gpsSerial.available() > 0) {
      char c = gpsSerial.read();
      if (c == '\n') {
        break; // End of NMEA sentence
      }
      nmeaSentence += c; // Append character to buffer
    }

    // Debug: Print raw NMEA sentence
    Serial.println("Raw NMEA Sentence:");
    Serial.println(nmeaSentence);

    // Process $GPGGA for latitude and longitude
    if (nmeaSentence.startsWith("$GPGGA")) {
      extractLatitudeLongitude(nmeaSentence);
    } else {
      Serial.println("Not a $GPGGA sentence. Ignoring...");
    }

  } else {
    Serial.println("No data from GPS module...");
    delay(1000); // Wait and retry
  }
}

// Function to extract latitude and longitude from $GPGGA
void extractLatitudeLongitude(const String &nmea) {
  Serial.println("Processing $GPGGA sentence...");

  String fields[15];
  splitNMEA(nmea, fields, 15);

  String latitude = fields[2];
  String latDirection = fields[3];
  String longitude = fields[4];
  String lonDirection = fields[5];

  if (latitude != "" && longitude != "") {
    Serial.print("Latitude: ");
    Serial.print(latitude + " " + latDirection);
    Serial.print(", Longitude: ");
    Serial.println(longitude + " " + lonDirection);
  } else {
    Serial.println("No valid GPS data in $GPGGA sentence.");
  }
}

// Helper function to split NMEA sentence into fields
void splitNMEA(const String &nmea, String *fields, int maxFields) {
  int fieldIndex = 0;
  int start = 0;
  int end = nmea.indexOf(',');

  while (end != -1 && fieldIndex < maxFields) {
    fields[fieldIndex++] = nmea.substring(start, end);
    start = end + 1;
    end = nmea.indexOf(',', start);
  }

  // Add last field (if any)
  if (fieldIndex < maxFields) {
    fields[fieldIndex] = nmea.substring(start);
  }
}
