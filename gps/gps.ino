#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// Pines para la comunicación con el GPS
static const int RXPin = D5, TXPin = D6; 
static const uint32_t GPSBaud = 9600; 

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println("GPS Test");
}

void loop() {
  // Leer datos del GPS
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  // Mostrar datos del GPS en el monitor serial
  if (gps.location.isValid()) {
    Serial.print("Latitud: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);
  } else {
    Serial.println("Localización GPS no disponible");
  }

  delay(1000); // Esperar un segundo antes de la próxima lectura
}
