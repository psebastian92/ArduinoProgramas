/*
  Capitulo 26 de Arduino desde cero en Español.
  Programa que permite mostrar en el monitor serial el codigo correspondiente al boton
  presionado en un control remoto mediante el modulo receptor KY-022. Ademas enciende
  con el boton 1 el componente rojo de un LED RGB catodo comun, con el boton 2 el verde
  y con el boton 3 el azul. Codigos de cada boton para un control especifico a modo
  de ejemplo.
  Codigo modificado para ser apto con version 3 y superior de la libreria.

  https://www.youtube.com/c/BitwiseAr
  Autor: bitwiseAr  

*/

#include <IRremote.h>     // importa libreria IRremote
#include <IRremote.h>
#include <IRremoteInt.h>
#define Boton_1 0x80BF49B6    // reemplaza cada instancia de Boton_1 por su valor HEX
#define Boton_2 0x80BFC936    // reemplaza cada instancia de Boton_2 por su valor HEX
#define Boton_3 0x80BF33CC    // reemplaza cada instancia de Boton_3 por su valor HEX

int SENSOR = 11;      // sensor KY-022 a pin digital 11

int LEDROJO = 2;      // componente rojo de LED RGB a pin 2
int LEDVERDE = 3;     // componente verde de LED RGB a pin 3
int LEDAZUL = 4;      // componente azul de LED RGB a pin 4

void setup() {
  Serial.begin(9600);     // inicializa comunicacion serie a 9600 bps
  IrReceiver.begin(SENSOR, DISABLE_LED_FEEDBACK); // inicializa recepcion de datos
  pinMode(LEDROJO, OUTPUT);   // pin 2 como salida
  pinMode(LEDVERDE, OUTPUT);    // pin 3 como salida
  pinMode(LEDAZUL, OUTPUT);   // pin 4 como salida
} 

void loop() {
  if (IrReceiver.decode()) {        // si existen datos ya decodificados
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // imprime valor en hexadecimal en monitor
    if (IrReceiver.decodedIRData.decodedRawData == Boton_1)   // si codigo recibido es igual a Boton_1
    digitalWrite(LEDROJO, !digitalRead(LEDROJO)); // enciende o apaga componente rojo

    if (IrReceiver.decodedIRData.decodedRawData == Boton_2)   // si codigo recibido es igual a Boton_2
    digitalWrite(LEDVERDE, !digitalRead(LEDVERDE)); // enciende o apaga componente verde

    if (IrReceiver.decodedIRData.decodedRawData == Boton_3)   // si codigo recibido es igual a Boton_3
    digitalWrite(LEDAZUL, !digitalRead(LEDAZUL)); // enciende o apaga componente azul
    
    IrReceiver.resume();        // resume la adquisicion de datos
  }
  delay (100);            // breve demora de 100 ms.
}
