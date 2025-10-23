// Define los pines para los motores
const int enableA = 5;  // Enable para Motor A
const int enableB = 6;  // Enable para Motor B
const int motorPin1 = 8; // Motor 1 dirección
const int motorPin2 = 9; // Motor 1 velocidad
const int motorPin3 = 11; // Motor 2 dirección
const int motorPin4 = 10; // Motor 2 velocidad

void setup() {
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
  // Avanzar gradualmente hacia adelante
  for (int i = 0; i <= 255; i++) {
    analogWrite(enableA, i);
    analogWrite(enableB, i);
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
    delay(12); // Controla la velocidad del incremento
  }

  delay(3000); // Mantiene la velocidad máxima durante 3 segundos

  // Retroceder gradualmente hacia atrás
  for (int i = 255; i >= 0; i--) {
    analogWrite(enableA, i);
    analogWrite(enableB, i);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
    delay(12); // Controla la velocidad del decremento
  }

  delay(3000); // Mantiene la velocidad máxima en retroceso durante 3 segundos
}

void stopCar() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  analogWrite(enableA, 0);
  analogWrite(enableB, 0);
}
