#include <Adafruit_Sensor.h>
#include <DHT.h>

// Pin donde está conectado el DHT11
#define DHTPIN 7     // Cambiá este número si usás otro pin

// Tipo de sensor
#define DHTTYPE DHT11

// Inicializar el sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);  // Monitor serie a 115200 baudios
  dht.begin();           // Inicializar el sensor
  Serial.println("Leyendo temperatura y humedad con DHT11 y ESP32...");
}

void loop() {
  delay(2000);  // Esperar 2 segundos entre lecturas

  // Leer humedad
  float h = dht.readHumidity();
  // Leer temperatura en Celsius
  float t = dht.readTemperature();

  // Comprobar si falló la lectura
  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer del sensor DHT11");
    return;
  }

  // Mostrar los datos
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" °C");
}
