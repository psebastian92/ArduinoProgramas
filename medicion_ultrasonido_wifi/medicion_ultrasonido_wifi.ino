#include <ESP8266WiFi.h>

// Definir los pines del sensor HC-SR04
const int trigPin = D1;
const int echoPin = D2;

// Variables para almacenar el tiempo y la distancia
long duration;
int distance;

// Definir el nombre y la contraseña de la red WiFi
const char* ssid = "TP-Link_3FE2";
const char* password = "26826615";

// Crear un objeto WiFiServer en el puerto 80
WiFiServer server(80);

void setup() {
  // Inicializar los pines del sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Inicializar la comunicación serial
  Serial.begin(115200);

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
  // Medir la distancia usando el sensor de ultrasonido
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

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
  Serial.println(request);
  client.flush();

  // Preparar la respuesta HTML
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "<h1>Distancia medida por el sensor de ultrasonido</h1>";
  s += "<p>";
  s += distance;
  s += " cm</p>";
  s += "</html>";

  // Enviar la respuesta al cliente
  client.print(s);
  delay(1);
  Serial.println("Cliente desconectado");
  Serial.println("");
}
