#include <WiFi.h>
#include <esp_now.h>

#define Depan 23
#define Belakang 21
#define Kiri 22
#define Kanan 19

// received data structure
typedef struct struct_message {
    char a[32];
    float b;
    float c;
} struct_message;
struct_message data_ku;
 
void setup() {
  Serial.begin(115200);  
// set esp to station mode
  WiFi.mode(WIFI_STA);

// initialize ESP_NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Gagal Inisialisasi espnow");
    return;
  }

  // register a callback function
  esp_now_register_recv_cb(cb_terima);

  // LED
  pinMode(Depan, OUTPUT);
  pinMode(Belakang, OUTPUT);
  pinMode(Kiri, OUTPUT);
  pinMode(Kanan, OUTPUT);
}


// callback if it receives data
void cb_terima(const uint8_t *mac_addr, const uint8_t *dataDiterima, int panjang) {
  memcpy(&data_ku, dataDiterima, sizeof(data_ku));
  Serial.print("Bytes diterima: ");
  Serial.println(panjang);
}

void loop() {
  Serial.print("Char: ");
  Serial.println(data_ku.a);
  Serial.print("Roll: ");
  Serial.print(data_ku.b);
  Serial.print(" Pitch: ");
  Serial.println(data_ku.c);

  // LED DIRECTION
  if (data_ku.b >= 30) {
    digitalWrite(Kanan, HIGH);
    digitalWrite(Depan, LOW);
    digitalWrite(Belakang, LOW);
    digitalWrite(Kiri, LOW);
    Serial.println("KANAN");
    Serial.println("");
    // The right LED is on
  } else if (data_ku.b <= -30) {
    digitalWrite(Kanan, LOW);
    digitalWrite(Depan, LOW);
    digitalWrite(Belakang, LOW);
    digitalWrite(Kiri, HIGH);
    Serial.println("KIRI");
    Serial.println("");
    // The left LED is on
  } else if (data_ku.c >= 30) {
    digitalWrite(Kanan, LOW);
    digitalWrite(Depan, HIGH);
    digitalWrite(Belakang, LOW);
    digitalWrite(Kiri, LOW);
    Serial.println("DEPAN");
    Serial.println("");
    // The front LED is on
  } else if (data_ku.c <= -30) {
    digitalWrite(Kanan, LOW);
    digitalWrite(Depan, LOW);
    digitalWrite(Belakang, HIGH);
    digitalWrite(Kiri, LOW);
    Serial.println("BELAKANG");
    Serial.println("");
    //  The back LED is on  
  } else {
    digitalWrite(Kanan, LOW);
    digitalWrite(Depan, LOW);
    digitalWrite(Belakang, LOW);
    digitalWrite(Kiri, LOW);
    Serial.println("DIAM");
    Serial.println("");
    // The LED is stationary because there is no movement from the MPU6050
  }
  delay(100);
}
