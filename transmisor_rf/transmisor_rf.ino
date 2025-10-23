#include <RH_ASK.h>        // Incluye librería RadioHead.h
#include <SPI.h>           // Incluye librería SPI necesaria por RadioHead.h

RH_ASK rf_driver(2000, 12, 11);  // Velocidad 2000 bps, pin TX 12, pin RX 11


void setup(){
    Serial.begin(9600);    // Inicia el monitor serie a 9600 bps
    rf_driver.init();      // Inicializa el objeto con valores por defecto
}

void loop(){
    const char *msg = "Hola desde lejos!";  // Puntero de mensaje a emitir
    rf_driver.send((uint8_t *)msg, strlen(msg));  // Envía el mensaje
    rf_driver.waitPacketSent();                    // Espera a que se complete la transmisión
    
    Serial.println("Mensaje enviado: Hola desde lejos!");  // Imprime confirmación
    delay(1000);               // Demora de 1 segundo entre envíos
}
