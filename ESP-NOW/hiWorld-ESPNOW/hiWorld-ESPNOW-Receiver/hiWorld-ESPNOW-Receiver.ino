#include <WiFi.h>
#include <esp_now.h>

// Estructura del mensaje (igual que en el emisor)
typedef struct struct_message {
  char msg[50];
} struct_message;

struct_message incomingData;

// Callback de recepción actualizado para ESP-IDF v5.x
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));

  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
          recv_info->src_addr[0], recv_info->src_addr[1], recv_info->src_addr[2],
          recv_info->src_addr[3], recv_info->src_addr[4], recv_info->src_addr[5]);

  Serial.print("Mensaje recibido de ");
  Serial.print(macStr);
  Serial.print(" -> ");
  Serial.println(incomingData.msg);
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  // Registrar callback de recepción
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Receptor listo, esperando mensajes...");
}

void loop() {
  // Nada, solo espera mensajes
}
