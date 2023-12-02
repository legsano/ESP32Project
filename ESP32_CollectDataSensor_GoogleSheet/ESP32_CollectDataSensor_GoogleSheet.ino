#include "TRIGGER_WIFI_ESP32.h"
#include "TRIGGER_GOOGLESHEETS_ESP32.h"
#include "DHT.h"
#include <HTTPClient.h> 

#define DHT11PIN 5
#define SOIL_MOISTURE_PIN 34
#define BLUE_LED_PIN 2 

DHT dht(DHT11PIN, DHT11);

char column_name_in_sheets[][20] = {"value1", "value2", "value3"};
String Sheets_GAS_ID = "AKfycbwkQuPhygjaRH-7tL4NG552GosExYgnJ4-twkqjSowcuiyrOAAQ3k1A8nKbEs5dz4UE"; //replace your gas ID which you got from the apps script here
int No_of_Parameters = 3;

const char* ssid = "Fern"; // Replace it with the SSID of your Wi-Fi network
const char* password = "1234567890e"; // Replace it with your Wi-Fi network password

unsigned long loopCount = 0; // Counter loop

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Set the blue LED pin as output
  pinMode(BLUE_LED_PIN, OUTPUT);

  WIFI_Connect(ssid, password);
  Google_Sheets_Init(column_name_in_sheets, Sheets_GAS_ID, No_of_Parameters);
  delay(5000);
}

void loop() {
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  delay(2000);
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  float soilMoisturePercent = map(soilMoistureValue, 0, 4095, 100, 0);


  if (loopCount >= 240) {
    Serial.println("Program telah berjalan 130 kali loop. Berhenti.");
    analogWrite(BLUE_LED_PIN, 100); // Turn on the blue light
    while (1) {
      // Stop with the endless loop
    }
  }

    if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi terputus. Mencoba menyambungkan kembali...");

    // Try connecting WiFi again
    WiFi.begin(ssid, password);

    // Wait until it connects to Wi-Fi
    while (WiFi.status() != WL_CONNECTED) {
      Serial.println("...");
      analogWrite(BLUE_LED_PIN, 100); // Turn on the blue light
      delay(1000);
    }

    Serial.println("Terhubung ke Wi-Fi kembali.");
    delay(10000);
  }

  

  // Create an HTTPClient object
  HTTPClient http;

  // Create a URL with parameters
  String url = "https://script.google.com/macros/s/" + Sheets_GAS_ID + "/exec";
  url += "?value1=" + String(soilMoisturePercent);
  url += "&value2=" + String(temp);
  url += "&value3=" + String(humi);

  // Send an HTTP GET request
  http.begin(url);

  int httpCode = http.GET(); //Execute the GET request

  http.end(); // End HTTP session

  if (httpCode == 200) {
    Serial.println("Gagal mengirim data ke Google Sheets.");
    Serial.print(soilMoisturePercent);
    Serial.print("   ");
    Serial.print(temp);
    Serial.print("   ");
    Serial.println(humi);
  } else {
    Serial.println("Data terkirim ke Google Sheets dengan sukses.");
    Serial.print(soilMoisturePercent);
    Serial.print("   ");
    Serial.print(temp);
    Serial.print("   ");
    Serial.println(humi);
  }

  // Add a time delay before sending the next data
  delay(60000);
  // Increment counter
  loopCount++;
}