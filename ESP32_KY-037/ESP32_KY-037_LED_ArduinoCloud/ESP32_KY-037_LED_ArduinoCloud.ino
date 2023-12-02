/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/73c3ba63-c890-44c0-8bcb-5849995082a8 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  int analog;
  bool lED;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "arduino_secrets.h"
#include "thingProperties.h"
#include "DHT.h"

#define KY037_ANALOG_PIN 35   // analog pin for KY-037 sensor
#define KY037_DIGITAL_PIN 4   // Digital pin for output KY-037 sensor
#define DHT11PIN 5            // analog pin for DHT11 sensor
#define LED_PIN 2             // Digital pin for LED 

DHT dht(DHT11PIN, DHT11);
int knockCount = 0;           // Number of knockCount

void setup() {
  // PINMODE
  pinMode(KY037_ANALOG_PIN, INPUT);
  pinMode(KY037_DIGITAL_PIN, INPUT);  // Set pin digital KY-037 sebagai input
  pinMode(LED_PIN, OUTPUT);

  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  int humidity = dht.readHumidity();          // Analog DHT11 readHumidity
  float temperature = dht.readTemperature();  // Analog DHT11 readTemperature
  Humi = humidity;
  Temp = temperature;
  Serial.print(Humi);
  Serial.print(" , ");
  Serial.println(Temp);

  // Your code here 
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
      lED = 1;
    } else if (knockCount == 2) {
      // Clap 2x, turn off the LED on the ESP32
      digitalWrite(LED_PIN, LOW);
      knockCount = 0; // Reset knockCount after 2 claps
      lED = 0;
    }
    // Wait a moment to avoid double detection
    delay(1000);
  }
  delay(500);
}

/*
  Since LED is READ_WRITE variable, onLEDChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onLEDChange()  {
  // Add your code here to act upon led change
  if (lED) {
    // Turn on LED
    digitalWrite(LED_PIN, HIGH);
    knockCount = 1;
  } else {
    // Turn off LED
    digitalWrite(LED_PIN, LOW);
    knockCount = 0;
  }
}

void onHumiChange()  {
  Serial.print("New Humidity value: ");
  Serial.println(Humi);
}

void onTempChange()  {
  Serial.print("New Temperature value: ");
  Serial.println(Temp);
}