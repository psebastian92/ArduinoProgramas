/*
  🚗 PROYECTO: Carro controlado por Bluetooth con sistema de seguridad y luces
  💡 AUTOR: [Tu nombre]
  🎓 PENSADO PARA: Estudiantes de 16 años (nivel intermedio)
  🧠 DESCRIPCIÓN:
    - El carro se controla desde una app Bluetooth (por ejemplo, "Arduino Bluetooth Controller").
    - Cada letra enviada indica una acción (avanzar, retroceder, girar, etc.).
    - Incluye luces delanteras y traseras controladas por comandos.
    - Si no se recibe ningún comando por 300 ms, el carro se detiene automáticamente por seguridad.
*/

#include <SoftwareSerial.h>  // Librería para usar comunicación serie en otros pines

// 🔹 Configuramos la conexión Bluetooth (RX, TX)
SoftwareSerial BT(5, 4); // RX = 5, TX = 4  (conecta el HC-05 a estos pines invertidos)

// 🔹 Pines del puente H (control de motores)
#define ENA 9   // Velocidad motor A (izquierdo)
#define IN1 7   // Sentido motor A
#define IN2 8
#define IN3 10  // Sentido motor B (derecho)
#define IN4 6
#define ENB 11  // Velocidad motor B

// 🔹 Luces
#define FRONT_LIGHTS 12 // Luces delanteras
#define BACK_LIGHTS 13  // Luces traseras

// 🔹 Velocidad base de los motores (0-255)
int velocidad = 220;  // 255 = velocidad máxima

// --- 🛡️ Variables de seguridad ---
unsigned long ultimoComando = 0;          // Guarda el tiempo del último comando recibido
const unsigned long tiempoSeguridad = 300; // Tiempo máximo sin señal (ms) antes de frenar

// --- 🧩 CONFIGURACIÓN INICIAL ---
void setup() {
  // Configuramos todos los pines como salidas
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(FRONT_LIGHTS, OUTPUT);
  pinMode(BACK_LIGHTS, OUTPUT);

  // Iniciamos comunicación serie (para ver mensajes en el monitor)
  Serial.begin(9600);

  // Iniciamos comunicación Bluetooth a 9600 baudios
  BT.begin(9600);

  // Mensaje de inicio
  Serial.println("🚗 Carro Bluetooth con auto-stop listo para usarse!");
}

// --- 🔁 BUCLE PRINCIPAL ---
void loop() {
  // Si hay datos disponibles desde el módulo Bluetooth...
  if (BT.available()) {
    char comando = BT.read();       // Leemos un carácter (comando)
    ultimoComando = millis();       // Actualizamos el tiempo del último comando
    ejecutarComando(comando);       // Ejecutamos la acción correspondiente
  }

  // 🛑 Si ha pasado demasiado tiempo sin recibir comandos, se frena automáticamente
  if (millis() - ultimoComando > tiempoSeguridad) {
    Serial.println("⚠️ Tiempo de seguridad superado → Frenando el auto");
    parar();  // Detiene los motores
  }
}

// --- 🎮 INTERPRETACIÓN DE COMANDOS ---
void ejecutarComando(char comando) {
  Serial.print("Comando recibido: ");
  Serial.println(comando);

  switch (comando) {
    case 'F': adelante(); break;              // Forward → Avanzar
    case 'B': atras(); break;                 // Backward → Retroceder
    case 'L': izquierda(); break;             // Left → Girar a la izquierda
    case 'R': derecha(); break;               // Right → Girar a la derecha
    case 'G': adelanteIzquierda(); break;     // Forward-Left
    case 'I': adelanteDerecha(); break;       // Forward-Right
    case 'H': atrasIzquierda(); break;        // Backward-Left
    case 'J': atrasDerecha(); break;          // Backward-Right
    case 'S': parar(); break;                 // Stop → Detenerse
    case 'W': digitalWrite(FRONT_LIGHTS, HIGH); break; // Enciende luces delanteras
    case 'w': digitalWrite(FRONT_LIGHTS, LOW); break;  // Apaga luces delanteras
    case 'U': digitalWrite(BACK_LIGHTS, HIGH); break;  // Enciende luces traseras
    case 'u': digitalWrite(BACK_LIGHTS, LOW); break;   // Apaga luces traseras
  }
}

// --- 🛞 FUNCIONES DE MOVIMIENTO ---

// 🔸 Avanzar hacia adelante
void adelante() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

// 🔸 Retroceder
void atras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

// 🔸 Girar a la izquierda (ambos motores giran en sentidos opuestos)
void izquierda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad / 2);
  analogWrite(ENB, velocidad / 2);
}

// 🔸 Girar a la derecha
void derecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad / 2);
  analogWrite(ENB, velocidad / 2);
}

// 🔸 Avanzar en diagonal izquierda
void adelanteIzquierda() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad / 2);  // Rueda izquierda más lenta
  analogWrite(ENB, velocidad);      // Rueda derecha normal
}

// 🔸 Avanzar en diagonal derecha
void adelanteDerecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);      // Rueda izquierda normal
  analogWrite(ENB, velocidad / 2);  // Rueda derecha más lenta
}

// 🔸 Retroceder en diagonal izquierda
void atrasIzquierda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad / 2);
  analogWrite(ENB, velocidad);
}

// 🔸 Retroceder en diagonal derecha
void atrasDerecha() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad / 2);
}

// 🔸 Detener completamente el carro
void parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
  