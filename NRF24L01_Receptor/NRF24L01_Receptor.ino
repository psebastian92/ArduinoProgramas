#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pines CE y CSN
#define CE_PIN 9
#define CSN_PIN 10

// Dirección del canal (debe coincidir con el transmisor)
byte direccion[5] = { 'c', 'a', 'n', 'a', 'l' };

// Objeto radio
RF24 radio(CE_PIN, CSN_PIN);

// Variable para los datos recibidos
float segundos;

void setup() {
  // Inicializa el radio
  radio.begin();

  // Configura canal de frecuencia 108 (2.508 GHz)
  radio.setChannel(108);

  // Configura velocidad y potencia
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);

  // Abre canal de lectura
  radio.openReadingPipe(1, direccion);

  // Comienza a escuchar
  radio.startListening();

  // Inicializa Serial
  Serial.begin(9600);
  Serial.println("Esperando datos...");
}

void loop() {
  // Verifica si hay datos
  if (radio.available()) {
    // Lee los datos (solo 1 float)
    radio.read(&segundos, sizeof(segundos));

    // Muestra en el monitor serie
    Serial.print("📥 Tiempo recibido: ");
    Serial.print(segundos);
    Serial.println(" segundos");
  } else {
    Serial.println("⏳ Esperando datos...");
  }

  delay(1000);
}
