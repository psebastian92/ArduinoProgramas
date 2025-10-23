#include <SoftwareSerial.h>

// Define los pines para los motores
const int motorPin1 = 8; // Motor 1 dirección
const int motorPin2 = 9; // Motor 1 velocidad
const int motorPin3 = 11; // Motor 2 dirección
const int motorPin4 = 10; // Motor 2 velocidad
const int enableA = 4;  // Enable para Motor A
const int enableB = 5; // Enable para Motor B

SoftwareSerial btSerial(2, 3); // RX, TX del módulo Bluetooth Slave

void setup() {
  Serial.begin(9600); // Configura el puerto serie para el Monitor Serial
  btSerial.begin(9600); // Configura el puerto serie del Bluetooth

  // Configura los pines de los motores
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // Inicialmente detiene el auto
  stopCar();
}

void loop() {
  if (btSerial.available()) {
    char command = btSerial.read(); // Lee el comando desde el Bluetooth
    Serial.print("Received command: "); // Imprime el comando recibido en el Monitor Serial
    Serial.println(command);

    switch (command) {
      case 'F':
        moveForward();
        break;
      case 'B':
        moveBackward();
        break;
      case 'L':
        turnLeft();
        break;
      case 'R':
        turnRight();
        break;
      case 'S':
        stopCar();
        break;
      default:
        stopCar(); // Detiene el auto si el comando no es reconocido
        break;
    }
  }
}

void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void moveBackward() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

void turnLeft() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

void turnRight() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void stopCar() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
