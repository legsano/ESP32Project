#define KY037_ANALOG_PIN 35   // Analog pin for KY-037 sensor
#define KY037_DIGITAL_PIN 4   // Digital pin for KY-037 sensor output
#define LED_PIN 2             // Digital pins for LED on ESP32

int knockCount = 0;    // Number of knock

void setup() {
  pinMode(KY037_ANALOG_PIN, INPUT);
  pinMode(KY037_DIGITAL_PIN, INPUT); // Set digital pin KY-037 as input
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int sensorValue = analogRead(KY037_ANALOG_PIN);
  Serial.println(sensorValue);

  // Detect the sound of hand clapping using the digital output of the KY-037
  if (digitalRead(KY037_DIGITAL_PIN) == HIGH) {
    knockCount++;
    Serial.print("Tepukan Tangan ke-");
    Serial.println(knockCount);

    if (knockCount == 1) {
      // Clap 1x, turn on the LED on the ESP32
      digitalWrite(LED_PIN, HIGH);
    } else if (knockCount == 2) {
      // Clap 2x, turn off the LED on the ESP32
      digitalWrite(LED_PIN, LOW);
      knockCount = 0; // Reset knockCount after 2 claps
    }

    // Wait a moment to avoid double detection
    delay(100);
  }
}
