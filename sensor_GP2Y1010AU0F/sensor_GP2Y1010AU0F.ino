#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Definición de pines
const int pinSensor = A0;
const int pinLed = 7;
const int adcMaximo = 1023;
const float voltajeVcc = 5.0;

// Pines para GPS (ajusta según tu conexión)
const int RXPin = 4, TXPin = 3;
const int GPSBaud = 9600;

// Inicialización de objetos GPS
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

float leerDensidad() {
  digitalWrite(pinLed, LOW);
  delayMicroseconds(280);
  int valorAdc = analogRead(pinSensor);  
  float voltajeSensor = voltajeVcc * valorAdc / adcMaximo;
  float densidad = 0.170 * voltajeSensor - 0.1;  // mg/m^3
  digitalWrite(pinLed, HIGH);
  return densidad * 1000;  // ug/m^3
}

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud); // Inicia la comunicación con el GPS
  pinMode(pinLed, OUTPUT);
  delay(1000);
}

void loop() {
  // Procesa la entrada del GPS
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  // Obtén la densidad de polvo
  Serial.print("Partículas disueltas en aire: ");
  Serial.print(leerDensidad());
  Serial.println(" ug/m^3");

  // Si hay datos válidos del GPS, imprímelos
  if (gps.location.isValid()) {
    Serial.print("Latitud: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);
  } else {
    Serial.println("Esperando datos de GPS...");
  }

  delay(1000);
}
