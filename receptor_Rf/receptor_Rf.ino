/*
	Capitulo 46 de Arduino desde cero en Español.
	Programa que recibe mensaje a traves del modulo receptor RF de 433 Mhz
	enviado desde el emisor. Ver Capitulo46-Programa1-Emisor.txt
	Requiere instalar libreria RadioHead.h

	Autor: bitwiseAr   

*/

//// Programa 1 lado Receptor ////

#include <RH_ASK.h>  // incluye libreria RadioHead.h
#include <SPI.h>     // incluye libreria SPI necesaria por RadioHead.h
          // crea objeto para modulacion por ASK
RH_ASK rf_driver(2000, 11, 12);  // Velocidad 2000 bps, pin RX 11, pin TX 12

void setup() {
  rf_driver.init();    // inicializa objeto con valores por defecto
  Serial.begin(9600);  // inicializa monitor serie a 9600 bps
}

void loop() {
  uint8_t buf[17];               // espacio para almacenar mensaje recibido
  uint8_t buflen = sizeof(buf);  // longitud del buffer de mensaje

  if (rf_driver.recv(buf, &buflen))  // si existen datos correctos recibidos
  {
    Serial.print("Mensaje: ");   // imprime Mensaje:
    Serial.println((char*)buf);  // imprime buffer de mensaje
  }
}