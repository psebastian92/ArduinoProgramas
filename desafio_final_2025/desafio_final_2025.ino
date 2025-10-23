#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Constantes para pines de conexión
const int pinJoyEjeY = A0;
const int pinServo = 4;
const int pinLedRojo = 9;
const int pinLedVerde = 7;
const int trigger = 3;
const int echo = 2;

// Constantes de calibración
const int anguloMin = 0;
const int anguloMax = 90;
const int distOcupado = 5;
const int distLibre = 7;

// Variables para usar en void loop
int valorJoy;
int angulo;
int brillo;
long duracion;
long distancia;
bool ocupado = false;
bool seEscribioBarreraBaja = false;
bool seEscribioPodesPasar = false;
bool seEscribioOcupado = false;
bool seEscribioLibre = false;

// Se configura servo y lcd
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // dirección 0x27 (a veces 0x3F)

// Constantes y variables para el fade in y fade out del led rojo
int brilloLedRojo = 0;
int pasoBrilloLedRojo = 5;
unsigned long ultimoTiempoFadeLedRojo = 0;
const int intervaloFadeLedRojoMs = 30;

// Constantes y variables para led rojo digitalmente
unsigned long ultimoTiempoParpadeoLedRojo = 0;  // marca de tiempo del último cambio
const int intervaloParpadeoLedRojoMs = 2000;    // intervalo entre cambios (ms)
bool ledRojoEncendido = false;                  // estado actual del LED rojo


void setup() {
  // Se definen pines de entrada y de salida
  pinMode(pinLedRojo, OUTPUT);
  pinMode(pinLedVerde, OUTPUT);
  pinMode(trigger, OUTPUT);
  pinMode(pinJoyEjeY, INPUT);
  pinMode(echo, INPUT);

  // Se configura el pin del servo con su código especial
  servo.attach(pinServo);

  // Encendido del LCD
  lcd.init();
  lcd.backlight();

  // Se imprime en pantalla LCD "Sistema listo"
  lcd.print("Sistema listo");

  //Espera de 2 segundos (2000 mili segundos)
  delay(2000);
}

void loop() {
  unsigned long tiempoActual = millis();
  valorJoy = analogRead(pinJoyEjeY);
  angulo = map(valorJoy, 0, 1023, anguloMin, anguloMax);

  servo.write(angulo);

  if (angulo < 10) {
    if (seEscribioBarreraBaja == false) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Barrera baja");
      seEscribioBarreraBaja = true;
      seEscribioPodesPasar = false;
    }

    if (tiempoActual - ultimoTiempoFadeLedRojo >= intervaloFadeLedRojoMs) {
      ultimoTiempoFadeLedRojo = tiempoActual;

      brilloLedRojo += pasoBrilloLedRojo;

      if (brilloLedRojo <= 0 || brilloLedRojo >= 255) {
        pasoBrilloLedRojo = -pasoBrilloLedRojo;
      }

      analogWrite(pinLedRojo, brilloLedRojo);
    }

  } else {
    if (seEscribioPodesPasar == false) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Podes pasar");
      seEscribioPodesPasar = true;
      seEscribioBarreraBaja = false;
    }
    if (tiempoActual - ultimoTiempoParpadeoLedRojo >= intervaloParpadeoLedRojoMs) {
      ultimoTiempoParpadeoLedRojo = tiempoActual;
      ledRojoEncendido = !ledRojoEncendido;  // alternar ON/OFF
      digitalWrite(pinLedRojoBarrera, ledRojoEncendido);
    }
  }

  // Medir distancia con ultrasonido
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duracion = pulseIn(echo, HIGH);
  distancia = duracion / 58;

  if (distancia <= distOcupado) {
    ocupado = true;
    digitalWrite(pinLedVerde, LOW);
    if (seEscribioOcupado == false) {
      lcd.setCursor(0, 1);
      lcd.print("Ocupado");
      seEscribioOcupado = true;
      seEscribioLibre = false;
    }

  } else if (distancia >= distLibre) {
    ocupado = false;
    digitalWrite(pinLedVerde, HIGH);

    if (seEscribioLibre == false) {
      lcd.setCursor(0, 1);
      lcd.print("Libre    ");
      seEscribioOcupado = false;
      seEscribioLibre = true;
    }
  }
}