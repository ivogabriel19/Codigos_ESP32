#include <WiFi.h>

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  String mac = WiFi.macAddress();  
  Serial.print("MAC Address (ESP32): ");
  Serial.println(mac);
}

void loop() {
  // Nada
}

