#include <Servo.h>

Servo myservo;  // Crea un objeto servo para controlar un servo motor

void setup() {
  myservo.attach(9);  // Asigna el servo al pin 9
}

void loop() {
  // Mueve el servo de 0 a 180 grados
  for (int pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);        // Establece la posición del servo
    delay(15);                 // Espera un breve periodo de tiempo para el movimiento
  }
  
  // Mueve el servo de 180 a 0 grados
  for (int pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);        // Establece la posición del servo
    delay(15);                 // Espera un breve periodo de tiempo para el movimiento
  }
}
