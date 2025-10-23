#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Configuración del LCD I2C (dirección típica 0x27, puede variar a 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Configuración del GPS en pines RX/TX (ejemplo: 4 y 3)
SoftwareSerial gpsSerial(4, 3);  // RX, TX
TinyGPSPlus gps;

boolean entroAlIf = false;

void setup() {
  // Inicialización del LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando GPS...");

  // Inicialización del GPS y Serial
  gpsSerial.begin(9600);     // Velocidad típica del NEO-6M
  Serial.begin(9600);        // Para depuración por monitor serial
  delay(2000);
  lcd.clear();

  Serial.println("Esperando señal GPS...");
}

void loop() {
  // Leer datos del GPS
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  // Si hay datos válidos, mostrar coordenadas
  if (gps.location.isUpdated()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lat:");
    lcd.print(gps.location.lat(), 6);

    lcd.setCursor(0, 1);
    lcd.print("Lon:");
    lcd.print(gps.location.lng(), 6);

    Serial.print("Latitud: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);
    Serial.println("----------------------");

    entroAlIf = true;
  } else {
    if (entroAlIf) {
      lcd.clear();
      entroAlIf = false;
    }
    lcd.setCursor(0, 0);
    lcd.print("Sin GPS");

    Serial.println("No hay señal GPS...");
    delay(2000);
  }
}
