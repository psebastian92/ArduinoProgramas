#include <SoftwareSerial.h>

// Configura el puerto serie para el módulo Bluetooth Master
SoftwareSerial btMaster(2, 3); // RX, TX

// Define los pines del joystick
const int joyXPin = A0; // Pin del eje X del joystick
const int joyYPin = A1; // Pin del eje Y del joystick
const int buttonPin = 8; // Pin del botón del joystick (si es aplicable)

// Define los límites de la zona muerta
const int deadZone = 100; // Puedes ajustar este valor según tus necesidades

// Variables para mantener el estado anterior de los comandos
char lastCommand = 'S';

void setup() {
  Serial.begin(9600); // Para el Monitor Serial
  btMaster.begin(9600); // Configura el puerto serie del Bluetooth
  pinMode(buttonPin, INPUT_PULLUP); // Configura el pin del botón como entrada con resistencia de pull-up
}

void loop() {
  int joyX = analogRead(joyXPin); // Lee el valor del eje X del joystick
  int joyY = analogRead(joyYPin); // Lee el valor del eje Y del joystick
  bool buttonState = digitalRead(buttonPin) == LOW; // Lee el estado del botón (presionado si LOW)

  // Imprime los valores del joystick en el Monitor Serial
  Serial.print("Joystick X: ");
  Serial.print(joyX);
  Serial.print("\tJoystick Y: ");
  Serial.print(joyY);
  Serial.print("\tButton State: ");
  Serial.println(buttonState ? "Pressed" : "Released");

  // Determina el comando a enviar basado en el joystick y el botón
  char newCommand = 'S'; // Por defecto es detener

  if (joyX < (512 - deadZone)) { // Mueve el joystick hacia la izquierda
    newCommand = 'L'; // Enviar comando para girar a la izquierda
  } else if (joyX > (512 + deadZone)) { // Mueve el joystick hacia la derecha
    newCommand = 'R'; // Enviar comando para girar a la derecha
  } else if (joyY < (512 - deadZone)) { // Mueve el joystick hacia adelante
    newCommand = 'B'; // Enviar comando para avanzar
  } else if (joyY > (512 + deadZone)) { // Mueve el joystick hacia atrás
    newCommand = 'F'; // Enviar comando para retroceder
  } else if (buttonState) { // Si el botón está presionado
    newCommand = 'S'; // Enviar comando para detener
  }

  // Solo envía el nuevo comando si ha cambiado
  if (newCommand != lastCommand) {
    btMaster.write(newCommand); // Enviar el nuevo comando al Bluetooth
    Serial.print("Sending: ");
    Serial.println(newCommand);
    lastCommand = newCommand; // Actualiza el comando anterior
  }

  delay(100); // Pequeña pausa para evitar el envío excesivo de comandos
}
