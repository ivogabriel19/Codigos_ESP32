#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "BananaSplit";           // Nombre del SSID
const char* password = "hola1234";            // Contraseña de la red

// Pines de los relés
#define RELAY1_PIN 32
#define RELAY2_PIN 33

// Estado inicial de los relés (apagados)
bool relay1State = false;
bool relay2State = false;

// Crear servidor en el puerto 80
WebServer server(80);

// HTML para la página web con dos botones
String getPage() {
  String page = "<html>\
  <head>\
    <title>Control de Relés</title>\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
    <style>body { font-family: Arial; text-align: center; } button { width: 100px; height: 50px; font-size: 18px; }</style>\
  </head>\
  <body>\
    <h1>Control de Relés</h1>";

  // Agregar estado de relé 1
  page += "<p>Relé 1 - Estado: " + String(relay1State ? "ON" : "OFF") + "</p>\
    <form action=\"/toggleRelay1\" method=\"POST\">\
      <button type=\"submit\">" + String(relay1State ? "Apagar" : "Encender") + "</button>\
    </form>";

  // Agregar estado de relé 2
  page += "<p>Relé 2 - Estado: " + String(relay2State ? "ON" : "OFF") + "</p>\
    <form action=\"/toggleRelay2\" method=\"POST\">\
      <button type=\"submit\">" + String(relay2State ? "Apagar" : "Encender") + "</button>\
    </form>";

  // Final del HTML
  page += "</body></html>";
  
  return page;
}

// Manejar la página principal
void handleRoot() {
  server.send(200, "text/html", getPage());
}

// Alternar el estado del relé 1
void handleToggleRelay1() {
  relay1State = !relay1State;
  digitalWrite(RELAY1_PIN, relay1State ? HIGH : LOW);  // Cambiar el estado del relé
  server.sendHeader("Location", "/");                  // Redirigir de vuelta a la página principal
  server.send(303);
  Serial.println("Escrito "+ String(relay1State) + " en el pin " + String(RELAY1_PIN));
}

// Alternar el estado del relé 2
void handleToggleRelay2() {
  relay2State = !relay2State;
  digitalWrite(RELAY2_PIN, relay2State ? HIGH : LOW);  // Cambiar el estado del relé
  server.sendHeader("Location", "/");                  // Redirigir de vuelta a la página principal
  server.send(303);
  Serial.println("Escrito "+ String(relay2State) + " en el pin " + String(RELAY2_PIN));
}

void setup() {
  Serial.begin(115200);

  // Configurar los pines de los relés como salida
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);

  // Apagar los relés al iniciar
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);

  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  Serial.println("Conectando a WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nConectado a la red WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configurar rutas del servidor
  server.on("/", handleRoot);                 // Página principal
  server.on("/toggleRelay1", HTTP_POST, handleToggleRelay1);  // Control del relé 1
  server.on("/toggleRelay2", HTTP_POST, handleToggleRelay2);  // Control del relé 2

  // Iniciar servidor
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();  // Manejar solicitudes de clientes
}
