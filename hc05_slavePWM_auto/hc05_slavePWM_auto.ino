#include <SoftwareSerial.h>

// Define los pines para los motores
const int enableA = 5;  // Enable para Motor A
const int enableB = 6;  // Enable para Motor B
const int motorPin1 = 8; // Motor 1 dirección
const int motorPin2 = 9; // Motor 1 velocidad
const int motorPin3 = 11; // Motor 2 dirección
const int motorPin4 = 10; // Motor 2 velocidad

SoftwareSerial btSerial(2, 3); // RX, TX del módulo Bluetooth Slave

void setup() {
  Serial.begin(9600); // Configura el puerto serie para el Monitor Serial
  btSerial.begin(9600); // Configura el puerto serie del Bluetooth

  // Configura los pines de los motores
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);

  // Inicialmente detiene el auto
  stopCar();
}

void loop() {
  if (btSerial.available() >= 2) {
    int valorX = btSerial.read(); // Lee el valor de X (0-255)
    int valorY = btSerial.read(); // Lee el valor de Y (0-255)

    Serial.print("Received X: ");
    Serial.print(valorX);
    Serial.print(" Y: ");
    Serial.println(valorY);

    controlCar(valorX, valorY);
  }
}

void controlCar(int x, int y) {
  // Control de Motor A (X eje)

  // izquierda
  if (x >= 0 && x <= 120) {
    analogWrite(enableA, map(x, 0, 120, 255, 0)); // Mapea el valor X para ir a la derecha
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    
  } else {
    analogWrite(enableA, 0);
  }

  // derecha
  if (x >= 130 && x <= 255) {
    analogWrite(enableB, map(x, 130, 255, 0, 255)); // Mapea el valor X para ir a la derecha
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
  } else {
    analogWrite(enableB, 0);
  }

  // abajo
  if (y >= 0 && y <= 120) {
    analogWrite(enableA, map(y, 0, 120, 255, 0)); // Mapea el valor X para ir a la derecha
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    analogWrite(enableB, map(y, 0, 120, 255, 0)); // Mapea el valor X para ir a la derecha
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
  }
  //arriba
  if (y >= 130 && y <= 255) {
    analogWrite(enableA, map(y, 130, 255, 0, 255)); // Mapea el valor X para ir a la derecha
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    analogWrite(enableB, map(y, 130, 255, 0, 255)); // Mapea el valor X para ir a la derecha
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);

  }
}

void stopCar() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  analogWrite(enableA, 0);
  analogWrite(enableB, 0);
}
