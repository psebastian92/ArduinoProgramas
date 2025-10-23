#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP_Google_Sheet_Client.h>

ESP_Google_Sheet_Client client;

const char *ssid = "Tu_SSID_WiFi";
const char *password = "Tu_Clave_WiFi";
const char *GSheetsID = "ID_de_Tu_Hoja_de_Calculo";
const char *GSheetsName = "Nombre_de_Tu_Hoja";
const char *GSheetsKey = "API_Key_de_Tu_Aplicacion"; // Se debe definir la clave de la API

const int moisturePin = A0;
int moistureValue = 0;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Conexión a la red Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red WiFi...");
  }
  Serial.println("Conexión WiFi establecida");

  // Inicialización de Google Sheets
  client.begin(GSheetsID, GSheetsName, GSheetsKey); // Se utiliza ESP_GSClient en lugar de GoogleSheets

  // Inicialización de pin analógico
  pinMode(moisturePin, INPUT);
}

void loop() {
  // Leer el valor de humedad
  moistureValue = analogRead(moisturePin);

  // Crear una fila de datos
  String dataRow = String(moistureValue);

  // Enviar los datos a Google Sheets
  if (client.appendRow(0, dataRow)) { // El primer parámetro es el índice de la hoja (0 si es la primera)
    Serial.println("Datos enviados a Google Sheets");
  } else {
    Serial.println("Error al enviar datos");
  }

  delay(60000);  // Esperar 1 minuto antes de enviar más datos
}
