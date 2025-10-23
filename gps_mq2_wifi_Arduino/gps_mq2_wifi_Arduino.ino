#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Define los pines RX y TX para la comunicación serial con el módulo GPS
const int RX_PIN = 10;  // Conectar TX del módulo GPS a 10
const int TX_PIN = 11;  // Conectar RX del módulo GPS a 11

// Inicializa el objeto SoftwareSerial para la comunicación con el módulo GPS
SoftwareSerial gpsSerial(RX_PIN, TX_PIN);

// Inicializa el objeto TinyGPS++
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);  // Inicialización de la comunicación serial para el monitor serial
  gpsSerial.begin(9600); // Inicialización de la comunicación serial para el módulo GPS
}

void loop() {
  bool newData = false;
  
  // Lee los datos del módulo GPS
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      newData = true;
    }
  }
  
  // Si se recibieron nuevos datos del GPS
  if (newData) {
    // Mostrar los datos del GPS en el monitor serial
    Serial.print("Latitud: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Velocidad: ");
    Serial.print(gps.speed.kmph());
    Serial.println(" km/h");
    Serial.print("Satélites: ");
    Serial.println(gps.satellites.value());
    Serial.print("Altitud: ");
    Serial.print(gps.altitude.meters());
    Serial.println(" metros");
    Serial.print("Fecha: ");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.println(gps.date.year());
    Serial.print("Hora: ");
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.println(gps.time.second());
    Serial.println("----------------------------------");
  } else {
    // Si no se recibieron datos del GPS
    Serial.println("No se recibió señal GPS.");
    delay(1000); // Espera para no saturar el monitor serial
  }
}
