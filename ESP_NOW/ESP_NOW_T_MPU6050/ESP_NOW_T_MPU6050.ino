#include <WiFi.h>
#include <esp_now.h>
#include <MPU6050.h>
#include <Wire.h>

MPU6050 mpu;



uint8_t mac_addr_tujuan[] = {0xAC, 0x67, 0xB2, 0x2D, 0x2C, 0xB0}; // Fill in the MAC address of your receiver

// sent data structure
typedef struct struct_message {
  char a[32];
  float b;
  float c;
} struct_message;
struct_message data_ku;

// peerInfo variable
esp_now_peer_info_t peerInfo;
 
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
  esp_now_register_send_cb(cb_terkirim);
  
  // Register Peers / Friends
  memcpy(peerInfo.peer_addr, mac_addr_tujuan, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add Peer / Friend
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Gagal Menambah Peer");
    return;
  }

  //MPU6050 initialize
  Wire.begin(21, 22);
  mpu.initialize();

  // Verify connection with sensor
  if (mpu.testConnection()) {
    Serial.println("MPU6050 terdeteksi!");
  } else {
    Serial.println("Koneksi dengan MPU6050 gagal.");
  }

}
 
// callback If data is sent
void cb_terkirim(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nStatus Pengiriman Paket Terakhir:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Pengiriman Sukses" : "Pengiriman Gagal");
}

void loop() {
  // MPU LOOP
  int16_t accelX, accelY, accelZ;
  mpu.getAcceleration(&accelX, &accelY, &accelZ);

  // Calculates tilt angle based on accelerometer data
  float roll = atan2(accelY, accelZ) * 180.0 / PI;
  float pitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180.0 / PI;

  // Determine the tilt direction
  String orientation = determineOrientation(roll, pitch);
  // Display data on Serial Monitor
  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.print("  Pitch: ");
  Serial.print(pitch);
  Serial.print("  Orientation: ");
  Serial.println(orientation);

  // Control led based on orientation

  // SEND TO RECIVER
  // assign value to variable
  strcpy(data_ku.a, "Dari Master");
  data_ku.b = roll;
  data_ku.c = pitch;
  // sent data
  esp_err_t result = esp_now_send(mac_addr_tujuan, (uint8_t *) &data_ku, sizeof(data_ku));

  delay(100);
}

String determineOrientation(float roll, float pitch) {
  if (roll > 30) {
    return "Kanan";
  } else if (roll < -30) {
    return "Kiri";
  } else if (pitch > 30) {
    return "Depan";
  } else if (pitch < -30) {
    return "Belakang";
  } else {
    return "Diam";
  }
}