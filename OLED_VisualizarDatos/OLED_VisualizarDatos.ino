#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ANCHO 128
#define ALTO 64
#define OLED_RESET 4

Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);


void setup() {
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.setTextSize(2);       // Tamaño del texto
  oled.setTextColor(WHITE);  // Color del texto
  oled.setCursor(0, 0);
  oled.println("Comenzando.");
  delay(1000);
  oled.display();
  oled.setCursor(0, 0);
  oled.println("Comenzando..");
  delay(1000);
  oled.display();
  oled.setCursor(0, 0);
  oled.println("Comenzando...");
  delay(1000);
  oled.display();
}

void loop() {
  oled.clearDisplay();   // Limpia pantalla
  oled.setCursor(0, 0);  // Posición inicial
  oled.println("T°: ");  // Muestra la frase
  oled.println("24°");
  oled.setCursor(40, 0);
  oled.println("Hum: ");  // Muestra la frase
  oled.println("66%");
  oled.display();
}
