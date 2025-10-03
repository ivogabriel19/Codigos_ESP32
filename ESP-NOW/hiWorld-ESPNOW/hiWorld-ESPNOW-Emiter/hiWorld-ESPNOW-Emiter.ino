#include <WiFi.h>
#include <esp_now.h>

// Dirección MAC del receptor
uint8_t broadcastAddress[] = {0xC0, 0x49, 0xEF, 0x68, 0x6F, 0x60};

// Estructura del mensaje
typedef struct struct_message {
  char msg[50];
} struct_message;

struct_message myData;

// Callback de envío compatible con ESP-IDF v5.x
void OnDataSent(const esp_now_send_info_t *info, esp_now_send_status_t status) {
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
          info->des_addr[0], info->des_addr[1], info->des_addr[2],
          info->des_addr[3], info->des_addr[4], info->des_addr[5]);

  Serial.print("Enviado a: ");
  Serial.print(macStr);
  Serial.print(" -> Estado: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Éxito" : "Fallo");
}

void setup() {
  Serial.begin(115200);

  // Configurar WiFi en modo estación
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Inicializar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  // Registrar callback de envío
  esp_now_register_send_cb(OnDataSent);

  // Registrar el peer (receptor)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Fallo al añadir el peer");
    return;
  }
}

void loop() {
  strcpy(myData.msg, "Hola desde ESP32 emisor!");

  // Enviar mensaje
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(2000);
}
