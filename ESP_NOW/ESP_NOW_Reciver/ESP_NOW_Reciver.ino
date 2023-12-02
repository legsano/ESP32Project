#include <WiFi.h>
#include <esp_now.h>

#define LED_PIN 2 // GPIO 2 (D2)

// received data structure
typedef struct struct_message {
    char a[32];
    bool b;
} struct_message;
struct_message data_ku;
 
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  // set esp to station mode
  WiFi.mode(WIFI_STA);

  // initialize ESP_NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Gagal Inisialisasi espnow");
    return;
  }

  // register a callback function
  esp_now_register_recv_cb(cb_terima);
}

// callback if it receives data
void cb_terima(const uint8_t *mac_addr, const uint8_t *dataDiterima, int panjang) {
  memcpy(&data_ku, dataDiterima, sizeof(data_ku));
  Serial.print("Bytes diterima: ");
  Serial.println(panjang);
  Serial.print("Char: ");
  Serial.println(data_ku.a);
  Serial.print("bool: ");
  Serial.println(data_ku.b);
  Serial.println(" ");

  digitalWrite(LED_PIN, data_ku.b); // Turn on lED
}

void loop() {

}
