#include <IRremote.h>  // Librería moderna para control remoto IR

// Definición pines EnA y EnB para el control de la velocidad
int VelocidadMotor1 = 6;
int VelocidadMotor2 = 5;

// Definición de los pines de control de giro de los motores In1, In2, In3 e In4
int Motor1A = 11;
int Motor1B = 10;
int Motor2C = 9;
int Motor2D = 8;

// Sensores infrarrojo
int infraPin = 2;
int infraPin1 = 4;

// Receptor IR
int receptorIR = 12;

// Variables de sensores
int valorInfra = 0;
int valorInfra1 = 0;

// Estado del modo automático
bool autoActivo = false;

// Código del botón OK (ajústalo según tu control)
#define BOTON_OK 0xE31CFF00

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(infraPin, INPUT);
  pinMode(infraPin1, INPUT);

  pinMode(Motor1A, OUTPUT);
  pinMode(Motor1B, OUTPUT);
  pinMode(Motor2C, OUTPUT);
  pinMode(Motor2D, OUTPUT);
  pinMode(VelocidadMotor1, OUTPUT);
  pinMode(VelocidadMotor2, OUTPUT);

  analogWrite(VelocidadMotor1, 150);
  analogWrite(VelocidadMotor2, 150);

  // Inicializa receptor IR
  IrReceiver.begin(receptorIR, ENABLE_LED_FEEDBACK);
  Serial.println("Receptor IR listo...");

  // Motores parados
  parar();
}

void loop() {
  valorInfra = digitalRead(infraPin);
  valorInfra1 = digitalRead(infraPin1);

  // Verificar si se recibe una señal IR
  if (IrReceiver.decode()) {
    uint32_t codigo = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("Código recibido: 0x");
    Serial.println(codigo, HEX);

    // Presionar OK enciende/apaga el modo automático
    if (codigo == BOTON_OK) {
      autoActivo = !autoActivo;  // Cambia el estado
      if (autoActivo) {
        Serial.println("🚗 Modo automático ACTIVADO");
      } else {
        Serial.println("🛑 Modo automático DESACTIVADO");
        parar();
      }
      delay(300); // Pequeña pausa para evitar doble lectura
    }

    // Si está detenido, permitir control manual con flechas
    if (!autoActivo) {
      switch (codigo) {
        case 0xE718FF00:  // Botón "UP"
          adelante(); delay(1000); parar(); break;
        case 0xAD52FF00:  // Botón "DOWN"
          atras(); delay(1000); parar(); break;
        case 0xF708FF00:  // Botón "LEFT"
          izquierda(); delay(1000); parar(); break;
        case 0xA55AFF00:  // Botón "RIGHT"
          derecha(); delay(1000); parar(); break;
      }
    }

    IrReceiver.resume();  // Limpia para el siguiente código
  }

  // --- Si el modo automático está activo ---
  if (autoActivo) {
    if (valorInfra == 0 && valorInfra1 == 0) {
      adelante();
    } 
    else if (valorInfra == 0 && valorInfra1 == 1) {
      derecha();
    } 
    else if (valorInfra == 1 && valorInfra1 == 0) {
      izquierda();
    } 
    else if (valorInfra == 1 && valorInfra1 == 1) {
      parar();
      Serial.println("Fin de línea, esperando orden...");
    }
  }
}

// ---- Funciones auxiliares ----
void adelante() {
  digitalWrite(Motor1A, HIGH);
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor2D, HIGH);
}

void atras() {
  digitalWrite(Motor1A, LOW);
  digitalWrite(Motor1B, HIGH);
  digitalWrite(Motor2C, HIGH);
  digitalWrite(Motor2D, LOW);
}

void izquierda() {
  digitalWrite(Motor1A, LOW);
  digitalWrite(Motor1B, HIGH);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor2D, HIGH);
}

void derecha() {
  digitalWrite(Motor1A, HIGH);
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, HIGH);
  digitalWrite(Motor2D, LOW);
}

void parar() {
  digitalWrite(Motor1A, LOW);
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor2D, LOW);
}
