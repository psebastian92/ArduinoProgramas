#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "TP-Link_3FE2";
const char *password = "26826615";
const char *serverAddress = "http://humedaddelsuelo.000webhostapp.com/recibirDatos.php";

int sensor = 0;
WiFiClient client;

void setup() {
  Serial.begin(9600);
  delay(10);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red WiFi...");
  }
  Serial.println("Conexión WiFi establecida");
  Serial.print("Dirección IP del dispositivo: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  String mensajeHumedad;
  sensor = analogRead(0);

  int porcentajeHumedad = (sensor - 1024) / -10.24;

  Serial.print(porcentajeHumedad);
  Serial.println("%");

  if (porcentajeHumedad < 40) {
    mensajeHumedad = "Regar!";
  } else {
    mensajeHumedad = "Tierra Húmeda";
  }

  enviarDatosAlServidor(mensajeHumedad, porcentajeHumedad);
  delay(60000); // Esperar 1 minuto antes de enviar más datos
}




void enviarDatosAlServidor(String mensajeHumedad, int porcentajeHumedad) {
  HTTPClient http;
  http.begin(client, serverAddress);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String enviarDatos = "mensajeHumedad=" + mensajeHumedad + "&porcentajeHumedad=" + String(porcentajeHumedad);

  int httpCodigoRespuesta = http.POST(enviarDatos);
  Serial.println(httpCodigoRespuesta);
  if (httpCodigoRespuesta > 0) {
    Serial.print("HTTP Código respuesta: ");
    Serial.println(httpCodigoRespuesta);
    if (httpCodigoRespuesta == 200) {
      String cuerpo_respuesta = http.getString();
      Serial.println("El servidor respondió: ");
      Serial.println(cuerpo_respuesta);
    }
  } else {
    Serial.print("Error en la solicitud: ");
    Serial.println(http.errorToString(httpCodigoRespuesta));
  }

  http.end();
}
