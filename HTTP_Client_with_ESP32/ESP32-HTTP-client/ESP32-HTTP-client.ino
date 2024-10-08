#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "IoTB";
const char* password = "inventaronelVAR";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a la red WiFi");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http; //comienza a crear una solicitud http

    // URL de la API de Randomuser.me
    http.begin("https://192.168.1.116.me/api/esp/list");
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Respuesta del servidor:");
      Serial.println(response);
    } else {
      Serial.print("Error en la solicitud HTTP: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
}

void loop() {
  // No se necesita código en el loop para este ejemplo
}
