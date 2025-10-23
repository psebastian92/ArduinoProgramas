#include <ESP8266WiFi.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// Definir el pin del sensor MQ-2
const int gasPin = A0;

// Variable para almacenar el valor del sensor MQ-2
int gasValue;

// Definir el nombre y la contraseña de la red WiFi
const char* ssid = "TP-Link_3FE2";
const char* password = "26826615";

// Crear un objeto WiFiServer en el puerto 80
WiFiServer server(80);

// Configuración de GPS
static const int RXPin = D5, TXPin = D6; // Pines para la comunicación con el GPS
static const uint32_t GPSBaud = 9600; // Velocidad de transmisión del GPS
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  // Inicializar el pin del sensor
  pinMode(gasPin, INPUT);

  // Inicializar la comunicación serial
  Serial.begin(115200);
  ss.begin(GPSBaud);

  // Conectar a la red WiFi
  Serial.println();
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Iniciar el servidor
  server.begin();
}

void loop() {
  // Leer el valor del sensor MQ-2
  gasValue = analogRead(gasPin);

  // Mostrar el valor del sensor de gas en el monitor serial
  Serial.print("Valor de gas: ");
  Serial.println(gasValue);

  // Procesar datos del GPS
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  // Mostrar los datos del GPS en el monitor serial
  if (gps.location.isValid()) {
    Serial.print("Latitud: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);
  } else {
    Serial.println("Localización GPS no disponible");
  }

  delay(1000);

  // Manejar las solicitudes del cliente
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("Nuevo cliente");
  while(!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println("Solicitud del cliente: " + request);
  client.flush();

  // Preparar la respuesta HTML
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "<h1>Medición de gas con sensor MQ-2</h1>";
  s += "<p>Valor de gas: ";
  s += gasValue;
  s += "</p>";
  if (gps.location.isValid()) {
    s += "<p>Latitud: ";
    s += gps.location.lat();
    s += "</p>";
    s += "<p>Longitud: ";
    s += gps.location.lng();
    s += "</p>";
  } else {
    s += "<p>Localización GPS no disponible</p>";
  }
  s += "</html>";

  // Enviar la respuesta al cliente
  client.print(s);
  delay(1);
  Serial.println("Respuesta enviada al cliente:");
  Serial.println(s);
  Serial.println("Cliente desconectado");
  Serial.println("");

  // Esperar un breve momento antes de la próxima medición
  delay(1000);
}
