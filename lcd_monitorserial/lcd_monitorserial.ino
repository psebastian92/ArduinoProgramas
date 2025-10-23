#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// Configuración de la pantalla LCD (Asegúrate de que los pines estén correctos)
Adafruit_LiquidCrystal lcd(0x3F);


const int ledPin = 13;  // Pin del LED
char command;         // Variable para almacenar el comando del monitor serial

void setup() {
  pinMode(ledPin, OUTPUT); // Configura el pin 13 como salida
  lcd.begin(16, 2);        // Inicia la pantalla LCD con 16 columnas y 2 filas
  lcd.setBacklight(HIGH);   // Configura la retroiluminación del LCD
  Serial.begin(9600);      // Inicia la comunicación serial
  lcd.print("LED apagado"); // Muestra el estado inicial
  Serial.println("Escribe '1' para encender el LED o '0' para apagarlo.");
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read(); // Lee el comando ingresado en el monitor serial

    if (command == "1") {
      digitalWrite(ledPin, HIGH);  // Enciende el LED
      lcd.clear();
      lcd.print("LED encendido");  // Muestra en el LCD
      Serial.println("LED encendido");
    } else if (command == "0") {
      digitalWrite(ledPin, LOW);   // Apaga el LED
      lcd.clear();
      lcd.print("LED apagado");    // Muestra en el LCD
      Serial.println("LED apagado");
    } else {
      Serial.println("Comando desconocido. Usa 'on' o 'off'.");
    }
  }
}
