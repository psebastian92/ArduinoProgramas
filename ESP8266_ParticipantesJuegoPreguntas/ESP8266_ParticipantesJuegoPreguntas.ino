#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define LAMPARA_PIN 2

#define PIN_BOTON D3  // Cambiá este pin si usás otro
#define NUM_EQUIPO 2 // Cambiá el número de equipo (1 a 6)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WebSocketsClient webSocket;

const char* ssid = "FliaMunoz";
const char* password = "26826615";
const char* host = "192.168.0.24";  // IP del ESP32 servidor

bool yaPresionado = false;

void setup() {
  pinMode(LAMPARA_PIN, OUTPUT);
  digitalWrite(LAMPARA_PIN, HIGH);  // Apagar pin
  Serial.begin(115200);
  pinMode(PIN_BOTON, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.println("Esperando...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  for (int i = 0; i <= 5; i++) {
    digitalWrite(LAMPARA_PIN, LOW);
    delay(500);
    digitalWrite(LAMPARA_PIN, HIGH);
    delay(500);
  }
  Serial.println("\nConectado al WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  webSocket.begin(host, 81, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(2000);
}

void loop() {
  webSocket.loop();

  if (digitalRead(PIN_BOTON) == LOW && !yaPresionado) {
    yaPresionado = true;
    String mensaje = "equipo:" + String(NUM_EQUIPO);
    webSocket.sendTXT(mensaje);
    Serial.println("Botón presionado, mensaje enviado: " + mensaje);
  }

  // Permitir volver a presionar después de soltar el botón
  if (digitalRead(PIN_BOTON) == HIGH) {
    yaPresionado = false;
  }
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    Serial.println("Mensaje recibido: " + msg);

    if (msg.indexOf(String(NUM_EQUIPO)) >= 0) {
      display.clearDisplay();
      display.setCursor(0, 20);
      display.setTextSize(2);
      display.println("¡Responde!");
      display.display();
    }

    if (msg == "led_on") {
      Serial.println("Encendiendo LED integrado...");
      digitalWrite(LAMPARA_PIN, LOW);   // Encender
      delay(3000);                      // Encendido por 3 segundos
      digitalWrite(LAMPARA_PIN, HIGH);  // Apagar
    }
  }
}
