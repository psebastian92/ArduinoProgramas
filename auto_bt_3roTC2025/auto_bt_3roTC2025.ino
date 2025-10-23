#include <SoftwareSerial.h>
SoftwareSerial BT(5, 4);

#define ENA 9
#define IN1 7
#define IN2 8
#define IN3 10
#define IN4 6
#define ENB 11

#define FRONT_LIGHTS 12
#define BACK_LIGHTS 13

int velocidad = 220; // Valor máx = 255

// --- Seguridad ---
unsigned long ultimoComando = 0;
const unsigned long tiempoSeguridad = 300; // ms sin señal -> frena

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(FRONT_LIGHTS, OUTPUT);
  pinMode(BACK_LIGHTS, OUTPUT);

  Serial.begin(9600);
  BT.begin(9600);
  Serial.println("Bluetooth RC Car con auto-stop");
}

void loop() {
  if (BT.available()) {
    char comando = BT.read();
    ultimoComando = millis();
    ejecutarComando(comando);
  }

  // Si no hay comando reciente -> detener
  if (millis() - ultimoComando > tiempoSeguridad) {
    Serial.println("Tiempo de seguridad superado. Frenando el auto");
    parar();
  }
}

void ejecutarComando(char comando) {
  Serial.print("Comando: ");
  Serial.println(comando);

  switch (comando) {
    case 'F': adelante(); break;
    case 'B': atras(); break;
    case 'L': izquierda(); break;
    case 'R': derecha(); break;
    case 'G': adelanteIzquierda(); break;
    case 'I': adelanteDerecha(); break;
    case 'H': atrasIzquierda(); break;
    case 'J': atrasDerecha(); break;
    case 'S': parar(); break;
    case 'W': digitalWrite(FRONT_LIGHTS, HIGH); break;
    case 'w': digitalWrite(FRONT_LIGHTS, LOW); break;
    case 'U': digitalWrite(BACK_LIGHTS, HIGH); break;
    case 'u': digitalWrite(BACK_LIGHTS, LOW); break;
  }
}

// --- Funciones de movimiento (idénticas a las tuyas) ---
void adelante() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}
void atras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}
void izquierda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad /2 );
  analogWrite(ENB, velocidad /2);
}
void derecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad /2);
  analogWrite(ENB, velocidad /2);
}
void adelanteIzquierda() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad / 2);
  analogWrite(ENB, velocidad);
}
void adelanteDerecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad / 2);
}
void atrasIzquierda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad / 2);
  analogWrite(ENB, velocidad);
}
void atrasDerecha() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad / 2);
}
void parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
