#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

// Configuración del sensor DHT
#define DHTPIN 15         // Pin al que está conectado el DHT
#define DHTTYPE DHT22    // Cambia a DHT11 si estás usando ese modelo
DHT dht(DHTPIN, DHTTYPE);

// Configuración de la red WiFi
const char* ssid = "BananaSplit";
const char* password = "hola1234";

// Configuración del servidor web
AsyncWebServer server(80);

// Variables para almacenar los valores de temperatura y humedad
float temperature;
float humidity;

// Página web que se mostrará
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; background-color: #f0f0f0; text-align: center; margin: 0; padding: 0; }
    h1 { color: #333; }
    .container { padding: 20px; margin-top: 50px; }
    .card { background-color: #fff; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); padding: 20px; border-radius: 8px; display: inline-block; }
    .label { font-size: 1.5em; color: #555; }
    .value { font-size: 2.5em; color: #007BFF; }
  </style>
</head>
<body>
  <div class="container">
    <div class="card">
      <h1>Sensor DHT22</h1>
      <p class="label">Temperatura:</p>
      <p class="value" id="temperature">%TEMPERATURE%°C</p>
      <p class="label">Humedad:</p>
      <p class="value" id="humidity">%HUMIDITY%%</p>
    </div>
  </div>
  <script>
    setInterval(function() {
      fetch('/data').then(function(response) {
        return response.json();
      }).then(function(data) {
        document.getElementById("temperature").innerHTML = data.temperature + "°C";
        document.getElementById("humidity").innerHTML = data.humidity + "%";
      });
    }, 5000); // Actualiza cada 5 segundos
  </script>
</body>
</html>
)rawliteral";

// Función para reemplazar los marcadores en el HTML
String processor(const String& var) {
  if (var == "TEMPERATURE") {
    return String(temperature);
  } else if (var == "HUMIDITY") {
    return String(humidity);
  }
  return String();
}

void setup() {
  // Inicia la conexión serial y el sensor DHT
  Serial.begin(115200);
  dht.begin();

  // Conexión a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a la red WiFi");

  // Configura el servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Endpoint para enviar los datos en formato JSON
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
    request->send(200, "application/json", json);
  });

  // Inicia el servidor
  server.begin();
}

void loop() {
  // Lee los valores del sensor DHT
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Verifica si la lectura fue exitosa
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error al leer el sensor DHT!");
    return;
  }

  // Imprime los valores en la consola para depuración
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" °C ");
  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.println(" %");
  
  // Espera antes de hacer una nueva lectura
  delay(2000);
}
