// RFID PORT
// VCC   --   3.3 V
// RST   --   22
// GND   --   GND
// IRQ   --   NOT USE
// MISO  --   19
// MOSI  --   23
// SCK   --   18
// SDA   --   21

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN    22
#define SS_PIN     21
#define LED_PIN    2 

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

// Allowed UID
byte allowedUID[] = {0x19, 0x07, 0xB8, 0x89};

void setup() {
  Serial.begin(115200);  // Initialize serial communications with the PC
  SPI.begin();           // Initiate SPI bus
  mfrc522.PCD_Init();    // Initiate MFRC522

  pinMode(LED_PIN, OUTPUT);  // Set LED_PIN as output
  Serial.println("Scan your RFID card...");
}

void loop() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Card detected!");

    // Show UID on serial monitor
    Serial.print("UID Value: ");
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    Serial.println(content);

    // Compare UID with Allowed UIDs
    if (compareUID(mfrc522.uid.uidByte)) {
      // LED is on for 5 seconds
      digitalWrite(LED_PIN, HIGH);
      delay(5000);
      digitalWrite(LED_PIN, LOW);
    } else {
      Serial.println("WRONG CARD");
    }

    delay(1000);
  }
}

// Function to compare UIDs
bool compareUID(byte* uid) {
  for (byte i = 0; i < sizeof(allowedUID); i++) {
    if (uid[i] != allowedUID[i]) {
      return false;  // UID does not match
    }
  }
  return true;  // UID matches
}
