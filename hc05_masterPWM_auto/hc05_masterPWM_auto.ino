#include <SoftwareSerial.h>
// Pines del joystick
const int pinX = A0;
const int pinY = A1;

SoftwareSerial btSerial(2, 3); // RX, TX para el módulo Bluetooth Master

void setup() {
  Serial.begin(9600); // Configura el puerto serie para el Monitor Serial
  btSerial.begin(9600); // Configura el puerto serie del Bluetooth
}

void loop() {
  // Leemos la posición del joystick
  int valorX = analogRead(pinX);
  int valorY = analogRead(pinY);

  // Convertimos los valores del joystick a un rango de 0-255 usando la función map
  int valorPWMX = map(valorX, 0, 1023, 0, 255); 
  int valorPWMY = map(valorY, 0, 1023, 0, 255);

  // Enviamos los valores X, Y por Bluetooth
  btSerial.write(valorPWMX); // Enviar valor X
  btSerial.write(valorPWMY); // Enviar valor Y

  // Debug en el Monitor Serial
  Serial.print("X: ");
  Serial.print(valorPWMX);
  Serial.print(" Y: ");
  Serial.println(valorPWMY);

  delay(200); // Reduzco el delay para mayor rapidez en la transmisión
}
