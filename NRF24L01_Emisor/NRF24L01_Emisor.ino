#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pines CE y CSN
#define CE_PIN 9
#define CSN_PIN 10

// Dirección del canal (5 bytes)
byte direccion[5] = { 'c', 'a', 'n', 'a', 'l' };

// Objeto radio
RF24 radio(CE_PIN, CSN_PIN);

// Datos a enviar
float segundos;

void setup() {
  // Inicializa el radio
  radio.begin();

  // Configura canal de frecuencia 108 (2.508 GHz)
  radio.setChannel(108);

  // Configura velocidad y potencia
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);

  // Abre canal de escritura
  radio.openWritingPipe(direccion);

  // Modo transmisión
  radio.stopListening();

  // Serial para debug
  Serial.begin(9600);
}

void loop() {
  // Simulación de datos

  segundos = millis() / 1000;                        

 Serial.println(segundos);

  // Enviar datos
  bool ok = radio.write(&segundos, sizeof(segundos));

  // Mostrar en monitor serie
  Serial.print("Segundos: ");
  Serial.println(segundos);

  if (ok) {
    Serial.println("✔ Enviado correctamente");
  } else {
    Serial.println("✘ Falló el envío");
  }

  Serial.println("---------------------------");
  delay(1000);
}
