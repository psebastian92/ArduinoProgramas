#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char *ssid = "TP-Link_3FE2";
const char *password = "26826615";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red WiFi...");
  }

  Serial.println("Conexión WiFi establecida");
  Serial.print("Dirección IP del dispositivo: ");
  Serial.println(WiFi.localIP());

  // Configurar el cliente de tiempo NTP
  timeClient.begin();
  timeClient.setTimeOffset(-3 * 60 * 60);  // Ajuste para la zona horaria de Argentina (GMT-3)
}

void loop() {
  timeClient.update();

  // Obtener la hora actual en formato de 24 horas
  int horaActual = timeClient.getHours();
  int minutoActual = timeClient.getMinutes();
  int segundoActual = timeClient.getSeconds();

  // Mostrar la hora en el monitor serial
  Serial.print("Hora actual: ");
  Serial.print(horaActual);
  Serial.print(":");
  Serial.print(minutoActual);
  Serial.print(":");
  Serial.println(segundoActual);

  delay(10000);  // Esperar 10 segundos antes de mostrar la hora nuevamente
}
