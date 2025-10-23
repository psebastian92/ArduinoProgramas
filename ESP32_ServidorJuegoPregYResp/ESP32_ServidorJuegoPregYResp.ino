#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>  
const char* ssid = "FliaMunoz";
const char* password = "26826615";

WebSocketsServer webSocket = WebSocketsServer(81);

String equipoPrimero = "";
int webClientID = -1;
int equipos[6] = { -1, -1, -1, -1, -1, -1 };                         // índice 0 = equipo 1, etc.
bool nuevoEquipo[6] = { false, false, false, false, false, false };  // Estado de si el equipo ha sido registrado

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado al WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}

void webSocketEvent(uint8_t clientID, WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    if (msg != "ping") {
      Serial.println("Mensaje recibido: " + msg);
    } else {
      webSocket.sendTXT(clientID, "pong");
    }

    // Procesar mensaje JSON
    if (msg.startsWith("{")) {
      // Intentar parsear el mensaje JSON
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, msg);

      if (error) {
        Serial.println("Error al parsear el mensaje JSON");
        return;
      }

      // Obtener los valores del JSON
      String accion = doc["accion"];
      String equipo = doc["equipo"];

      if (accion == "encenderLED") {
        // Accion: encenderLED
        Serial.println("Encender LED para el equipo " + equipo);

        // Encuentra el equipo correspondiente y envía el mensaje al ESP8266
        int numeroEquipo = equipo.toInt();
        if (numeroEquipo >= 1 && numeroEquipo <= 6 && equipos[numeroEquipo - 1] != -1) {
          webSocket.sendTXT(equipos[numeroEquipo - 1], "led_on");
        }
      }
    }

    // Registro de equipos
    if (msg.startsWith("equipo:")) {
      int numEquipo = msg.substring(7).toInt();
      if (numEquipo >= 1 && numEquipo <= 6) {
        if (!nuevoEquipo[numEquipo - 1]) {  // Si el equipo no ha sido registrado
          equipos[numEquipo - 1] = clientID;
          nuevoEquipo[numEquipo - 1] = true;  // Marcar equipo como registrado
          Serial.println("Equipo " + String(numEquipo) + " registrado con clientID " + String(clientID));

          // Notificar al cliente Web que un nuevo equipo ha sido registrado
          if (webClientID != -1) {
            String msgRegistro = "Nuevo equipo: " + String(numEquipo) + " registrado!";
            webSocket.sendTXT(webClientID, msgRegistro);
          }
        }
      }
    }

    // Registro del cliente web
    if (msg == "web") {
      webClientID = clientID;
      Serial.println("Cliente web registrado con ID: " + String(webClientID));

      // Enviar los equipos registrados (esto sirve sobre todo si se reinicia la página)
      for (int i = 0; i < 6; i++) {
        if (nuevoEquipo[i]) {
          String msgRegistro = "Nuevo equipo: " + String(i + 1) + " registrado!";
          webSocket.sendTXT(webClientID, msgRegistro);
          Serial.print("WEB actualizada. Equipo reenviado:");
          Serial.println(equipos[i]);
        }
      }
    }

    // Presionar un equipo
    if (msg.startsWith("equipo:")) {
      equipoPrimero = msg.substring(7);
      if (equipoPrimero == "1" || equipoPrimero == "2" || equipoPrimero == "3" || equipoPrimero == "4" || equipoPrimero == "5" || equipoPrimero == "6") {
        Serial.println("¡Presionó el equipo " + equipoPrimero + "!");

        // Avisar al cliente web
        if (webClientID != -1) {
          webSocket.sendTXT(webClientID, "Presionó el equipo " + equipoPrimero);
        }

      }
    }

    // Reiniciar juego
    if (msg == "reset") {
      equipoPrimero = "";
      Serial.println("Juego reiniciado.");
    }
  }
}
