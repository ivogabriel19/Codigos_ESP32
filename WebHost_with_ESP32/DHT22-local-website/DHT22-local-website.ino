#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// Configuración del DHT
#define DHTPIN 15  // Pin donde está conectado el sensor
#define DHTTYPE DHT22  // Cambia a DHT11 si estás usando un DHT11

DHT dht(DHTPIN, DHTTYPE);

// Configuración WiFi
const char* ssid = "BananaSplit";
const char* password = "hola1234";

// Crear un objeto de servidor web en el puerto 80
WebServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado a WiFi. Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configurar las rutas del servidor web
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  // Manejar solicitudes HTTP
  server.handleClient();
}

// Función para manejar la solicitud a la raíz ("/")
void handleRoot() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    server.send(500, "text/plain", "Error al leer el sensor DHT");
    return;
  }

  String html = "<!DOCTYPE html><html>";
  html += "<head><meta http-equiv='refresh' content='10'/>";
  html += "<title>ESP32 DHT Server</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; background-color: #f0f8ff; text-align: center; padding: 50px;}";
  html += ".container { background-color: #ffffff; padding: 20px; border-radius: 10px; box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1); max-width: 400px; margin: auto;}";
  html += "h1 { color: #333333; }";
  html += "p { font-size: 1.5em; margin: 10px 0; }";
  html += "</style></head>";
  html += "<body>";
  html += "<div class='container'>";
  html += "<h1>Valores de Temperatura y Humedad</h1>";
  html += "<p>🌡️ Temperatura: <strong>" + String(t) + " °C</strong></p>";
  html += "<p>💧 Humedad: <strong>" + String(h) + " %</strong></p>";
  html += "</div>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}
