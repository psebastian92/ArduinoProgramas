// Incluimos librería e instanciamos el objeto servo
#include <Servo.h>
Servo myservo;

// Definición pines EnA y EnB para el control de la velocidad
int VelocidadMotor1 = 5;
int VelocidadMotor2 = 6;

// Definición pines sensor distancia y variables para el cálculo de la distancia
int echoPin = 2;
int trigPin = 3;
long duration;
int distance;
int delayVal;

// Definición de los pines de control de giro de los motores In1, In2, In3 e In4
int Motor1A = 13;
int Motor1B = 12;
int Motor2C = 8;
int Motor2D = 10;

// Variable control posición servo y observaciones
int servoPos = 0;
int servoReadLeft = 0;
int servoReadRight = 0;
int detectorVeces = 0;

// Configuración inicial
void setup() {
  delay(1000);

  // Vinculamos el pin digital 9 al servo instanciado arriba
  myservo.attach(9);

  // Establecemos modo de los pines del sensor de ultrasonidos
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Establecemos modo de los pines del control de motores
  pinMode(Motor1A, OUTPUT);
  pinMode(Motor1B, OUTPUT);
  pinMode(Motor2C, OUTPUT);
  pinMode(Motor2D, OUTPUT);
  pinMode(VelocidadMotor1, OUTPUT);
  pinMode(VelocidadMotor2, OUTPUT);

  // Configuramos velocidad de los dos motores
  analogWrite(VelocidadMotor1, 121 );
  analogWrite(VelocidadMotor2, 100);

  myservo.write(90);
  Serial.begin(9600);

}

// Ejecución contínua
void loop() {
  delay(50);

  distance = medirDistancia();
  Serial.println(distance);

  if (distance < 25) {
    stopCar();

    // Miramos a la derecha
    myservo.write(10);
    delay(600);
    servoReadRight = medirDistancia();

    // Miramos a la izquierda
    myservo.write(170);
    delay(600);
    servoReadLeft = medirDistancia();

    // Miramos de frente
    myservo.write(90);
    delay(600);

    if (servoReadLeft > servoReadRight) {
      Serial.println("Giro izquierda");
      turnLeftCar();
    }

    if (servoReadRight >= servoReadLeft) {
      Serial.println("Giro derecha");
      turnRightCar();
    }

    detectorVeces++;
    if (detectorVeces == 4) {
      moveBackwardCar();
      Serial.println("Atras");
      delay(3000);
      stopCar();
      detectorVeces = 0;
    }

  }

  if (distance > 25) {
    Serial.println("Recto");
    moveForwardCar();
  }

}

void stopCar() {
  // Paramos el carrito
  digitalWrite(Motor1A, LOW);
  digitalWrite(Motor2D, LOW);
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, LOW);

}

void turnRightCar() {
  // Configuramos sentido de giro para dirar a la derecha
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor1B, HIGH);
  digitalWrite(Motor2C, LOW);
  delay(250);
}

void turnLeftCar() {
  // Configuramos sentido de giro para dirar a la izquierda
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, HIGH);
  delay(250);
}

void moveForwardCar() {
  // Configuramos sentido de giro para avanzar
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor1B, HIGH);
  digitalWrite(Motor2C, HIGH);
}

void moveBackwardCar() {
  // Configuramos sentido de giro para avanzar
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor1A, HIGH);
  digitalWrite(Motor2D, HIGH);
  delay(2000);
}

int medirDistancia() {
  // Lanzamos pulso de sonido
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Leemos lo que tarda el pulso en llegar al sensor y calculamos distancia
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Devolver distancia calculada
  return distance;
}
