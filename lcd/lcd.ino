#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Crea el objeto LCD con la dirección encontrada (0x3F), 16 columnas y 2 filas
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  lcd.init();              // Inicializa el LCD
  lcd.backlight();         // Enciende la luz de fondo
  lcd.setCursor(0, 0);     // Columna 0, fila 0
  lcd.print("Hola Mundo!");
  lcd.setCursor(0, 1);     // Columna 0, fila 1
  lcd.print("LCD I2C OK!");
}

void loop() {
  // Nada en el loop, solo muestra el texto
}
