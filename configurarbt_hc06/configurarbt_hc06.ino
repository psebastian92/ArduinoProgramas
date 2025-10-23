#include <SoftwareSerial.h>

// Define los pines para la comunicación serial con el HC-06
SoftwareSerial blue(2, 3);   // RX, TX

void setup() {
    Serial.begin(9600);   // Configura el monitor serial a 9600 baudios
    blue.begin(9600);     // Configura el puerto serial para el HC-06 a 9600 baudios

    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH); // Enciende el LED 13 durante 4 segundos antes de consultar el Bluetooth
    delay(4000);

    digitalWrite(13, LOW); // Apaga el LED 13 para iniciar la consulta

    // Consulta el nombre del dispositivo
    blue.println("AT+NAME?");  
    delay(1000);

    // Consulta la velocidad de baudios
    blue.println("AT+BAUD?");  
    delay(1000);

    // Consulta el PIN del dispositivo (algunos módulos HC-06 pueden no responder a este comando)
    blue.println("AT+PIN?");  
    delay(1000);
}

void loop() {
    // Leer datos desde el HC-06 y enviarlos al monitor serial
    if (blue.available()) {
        char c = blue.read();
        Serial.print(c); // Imprime datos recibidos del HC-06 en el monitor serial
    }

    // Leer datos desde el monitor serial y enviarlos al HC-06 (en caso de que necesites enviar datos al HC-06)
    if (Serial.available()) {
        char c = Serial.read();
        blue.print(c); // Envia datos recibidos del monitor serial al HC-06
    }

    // Parpadea el LED 13 para indicar que el código está en ejecución
    digitalWrite(13, !digitalRead(13));
    delay(300);
}
