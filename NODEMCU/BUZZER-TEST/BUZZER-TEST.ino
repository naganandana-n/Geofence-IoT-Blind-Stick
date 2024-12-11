/*

#define buzzerPin D6 // Buzzer Signal Pin on GPIO12

void setup() {
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as OUTPUT
  Serial.begin(115200);       // Initialize Serial Monitor
  Serial.println("Buzzer Test Starting...");
}

void loop() {
  Serial.println("Buzzer ON");
  digitalWrite(buzzerPin, HIGH); // Turn buzzer ON
  delay(1000);                  // Wait 1 second

  Serial.println("Buzzer OFF");
  digitalWrite(buzzerPin, LOW);  // Turn buzzer OFF
  delay(1000);                  // Wait 1 second
}

*/

#define buzzerPin D6 // GPIO12

void setup() {
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Testing Passive Buzzer...");
}

void loop() {
  Serial.println("Buzzer Sounding...");
  for (int i = 0; i < 500; i++) { // This is the loudest and most clear we could make the sound
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(125);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(125);
  }
  delay(1000); // Pause for 1 second
}