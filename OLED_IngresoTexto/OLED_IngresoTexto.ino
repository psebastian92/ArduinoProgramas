#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ANCHO 128
#define ALTO 64
#define OLED_RESET 4

Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);

String frase = "";

void setup() {
  Serial.begin(9600);
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.setTextSize(2);             // Tamaño del texto
  oled.setTextColor(WHITE);        // Color del texto
  oled.setCursor(0, 0);
  oled.println("Escribí una frase:");
  oled.display();
}

void loop() {
  if (Serial.available()) {
    frase = Serial.readStringUntil('\n');  // Lee hasta Enter

    oled.clearDisplay();                   // Limpia pantalla
    oled.setCursor(0, 0);                  // Posición inicial
    oled.println(frase);                  // Muestra la frase
    oled.display();
  }
}
