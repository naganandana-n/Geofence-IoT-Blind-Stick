/*

#define irSensorPin D3  // IR Sensor connected to GPIO D3
#define buzzerPin D6    // Passive Buzzer connected to GPIO D6

void setup() {
  pinMode(irSensorPin, INPUT);   // Set IR sensor pin as INPUT
  pinMode(buzzerPin, OUTPUT);    // Set buzzer pin as OUTPUT
  digitalWrite(buzzerPin, LOW);  // Ensure buzzer is off initially
  
  Serial.begin(115200);          // Start Serial Monitor for debugging
  Serial.println("IR Sensor and Buzzer Test Starting...");
}

void loop() {
  int irSensorValue = digitalRead(irSensorPin); // Read IR sensor value

  // Check if IR sensor detects ground/contact (LOW state)
  if (irSensorValue == LOW) {
    Serial.println("IR Sensor: Ground Detected! Beeping...");
    triggerBuzzer(); // Activate buzzer
  } else {
    Serial.println("IR Sensor: No Ground Detected");
    digitalWrite(buzzerPin, LOW); // Ensure buzzer is off
  }

  delay(500); // Small delay for better serial readability
}

// Function to trigger the buzzer manually at 1kHz
void triggerBuzzer() {
  for (int i = 0; i < 500; i++) { 
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(125); // Half cycle of 1kHz (500µs HIGH)
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(125); // Half cycle of 1kHz (500µs LOW)
  }
}

*/

#define irSensorPin D3  // IR Sensor connected to GPIO D3
#define buzzerPin D6    // Passive Buzzer connected to GPIO D6

void setup() {
  pinMode(irSensorPin, INPUT);   // Set IR sensor pin as INPUT
  pinMode(buzzerPin, OUTPUT);    // Set buzzer pin as OUTPUT
  digitalWrite(buzzerPin, LOW);  // Ensure buzzer is off initially
  
  Serial.begin(9600);          // Start Serial Monitor for debugging
  Serial.println("IR Sensor and Buzzer Test Starting...");
}

void loop() {
  int irSensorValue = digitalRead(irSensorPin); // Read IR sensor value

  // Check if IR sensor detects ground/contact (LOW state)
  if (irSensorValue == LOW) {
    Serial.println("IR Sensor: Ground Detected! Beeping...");
    triggerBuzzer(); // Activate buzzer continuously
  } else {
    Serial.println("IR Sensor: No Ground Detected");
    digitalWrite(buzzerPin, LOW); // Ensure buzzer is off
  }
}

// Function to trigger the buzzer continuously while condition holds
void triggerBuzzer() {
  // THE PROBLEM WITH THIS IS THAT IT CAUSES THE NODEMCU TO RESET!
  /*
  while (digitalRead(irSensorPin) == LOW) { // Continue as long as object is detected
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(125); // Half cycle for 1kHz
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(125);
  }
  Serial.println("Buzzer stopped, no object detected.");
  */

  // SOLUTION:
  for (int i = 0; i < 500; i++) { 
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(125); // Half cycle of 1kHz (500µs HIGH)
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(125); // Half cycle of 1kHz (500µs LOW)
  }
}