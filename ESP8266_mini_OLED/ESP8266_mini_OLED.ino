#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ANCHO_PANTALLA 128
#define ALTO_PANTALLA 64
#define OLED_RESET    -1  // Reset no conectado
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  // Inicializar pantalla
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Dirección I2C 0x3C por defecto
    Serial.println(F("No se encontró pantalla OLED"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(3);             // Tamaño de letra
  display.setTextColor(SSD1306_WHITE); // Color blanco
  display.setCursor(10, 20);           // Posición en la pantalla
  display.println("HOLA");            // Texto a mostrar
  display.display();                  // Mostrar en pantalla
}

void loop() {
  // Nada por ahora
}
