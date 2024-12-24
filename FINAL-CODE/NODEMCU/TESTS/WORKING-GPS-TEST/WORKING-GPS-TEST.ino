#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define gpsRxPin D1
#define gpsTxPin D2
SoftwareSerial neo6m(gpsTxPin, gpsRxPin);

TinyGPSPlus gps;

void setup()
{
  Serial.begin(9600); // Start serial communication for debugging
  Serial.println("GPS Address Printing Example");
 
  neo6m.begin(9600); // Start serial communication with GPS module
}

void loop()
{
  smartdelay_gps(1000); // Collect GPS data with a delay

  if (true)
  {
    // Fetch latitude and longitude
    double latitude = gps.location.lat();
    double longitude = gps.location.lng();

    // Print coordinates to Serial Monitor
    Serial.print("Latitude: ");
    Serial.println(latitude, 6); // Print latitude with 6 decimal places
    Serial.print("Longitude: ");
    Serial.println(longitude, 6); // Print longitude with 6 decimal places
  }
  else
  {
    Serial.println("Waiting for valid GPS data...");
  }
}

static void smartdelay_gps(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (neo6m.available())
      gps.encode(neo6m.read());
  } while (millis() - start < ms);
}