#include <WiFi.h>
#include <WebSocketsServer.h>
#include <WiFiManager.h> // <<--- NUEVO

WebSocketsServer webSocket(81);

void webSocketEvent(uint8_t clientID, WStype_t type, uint8_t* payload, size_t length); // <<--- AVISO QUE EXISTE

void setup() {
  Serial.begin(115200);

  // --- NUEVO WiFiManager ---
  WiFiManager wifiManager;
  wifiManager.autoConnect("Configurar JuegoESP"); 
  // Crea un AP si no se puede conectar, llamado "Configurar JuegoESP"

  Serial.println("WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  // --------------------------

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}

// ------------------------------------------------------------
// Copia aquí toda tu función webSocketEvent de tu primer archivo
// ------------------------------------------------------------

void webSocketEvent(uint8_t clientID, WStype_t type, uint8_t* payload, size_t length) {
  // TODO: copia aquí todo lo que tenías antes en webSocketEvent
}
