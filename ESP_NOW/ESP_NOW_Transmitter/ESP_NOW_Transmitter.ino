#include <WiFi.h>
#include <esp_now.h>


uint8_t mac_addr_tujuan[] = {0xAC, 0x67, 0xB2, 0x2D, 0x2C, 0xB0};  // Fill in the MAC address of your receiver

// sent data structure
typedef struct struct_message {
  char a[32];
  bool b;
} struct_message;
struct_message data_ku;

// peerInfo variable
esp_now_peer_info_t peerInfo;

// callback If data is sent
void cb_terkirim(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nStatus Pengiriman Paket Terakhir:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Pengiriman Sukses" : "Pengiriman Gagal");
}
 
void setup() {
  Serial.begin(115200);

  /// set esp to station mode
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
}
 
void loop() {
  esp_err_t result = esp_now_send(mac_addr_tujuan, (uint8_t *) &data_ku, sizeof(data_ku));
  // assign value to variable
  strcpy(data_ku.a, "Dari Master");
  data_ku.b = true;
  // sent data
  result = esp_now_send(mac_addr_tujuan, (uint8_t *) &data_ku, sizeof(data_ku));
  delay(2000);

  strcpy(data_ku.a, "Dari Master");
  data_ku.b = false;
  result = esp_now_send(mac_addr_tujuan, (uint8_t *) &data_ku, sizeof(data_ku));
  delay(2000);
}
