#include <SoftwareSerial.h>

// Define los pines de SoftwareSerial para el HC-05
SoftwareSerial btSerial(2, 3); // RX, TX

void setup() {
  // Configura el monitor serial para comunicarte con el PC
  Serial.begin(9600);
  // Configura el puerto serie para el HC-05
  btSerial.begin(9600);

  Serial.println("Enter AT commands:");
}

void loop() {
  // Si recibes datos desde el monitor serial de la PC, los envías al HC-05
  if (Serial.available()) {
    btSerial.write(Serial.read());
  }

  // Si recibes datos desde el HC-05, los envías al monitor serial de la PC
  if (btSerial.available()) {
    Serial.write(btSerial.read());
  }
}
