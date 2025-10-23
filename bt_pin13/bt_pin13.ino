#include <SoftwareSerial.h>

const int pin13 = 13;
SoftwareSerial bluetooth(2, 3); 
// RX en pin 2 de arduino y TX del módulo Bluetooth. 
// TX en pin 3 de arduino y RX del módulo Bluetooth. 

void setup() {
  pinMode(pin13, OUTPUT);

  // Inicializa la comunicación serial para el monitor serial de la PC
  Serial.begin(9600);

  // Inicializa la comunicación serial en los pines 2 (RX) y 3 (TX) para el módulo Bluetooth
  bluetooth.begin(9600);

  // Mensaje de bienvenida en el monitor serial y el módulo Bluetooth
  Serial.println("Bienvenido!");
  bluetooth.println("Bienvenido!");

  delay(2000); // Espera 2 segundos
}

void loop() {
  // Mensajes de instrucciones en el monitor serial y el módulo Bluetooth
  Serial.println("Ingrese 1 para encender el LED");
  Serial.println("Ingrese 0 para apagar el LED");
  bluetooth.println("Ingrese 1 para encender el LED");
  bluetooth.println("Ingrese 0 para apagar el LED");

  // Espera a que se reciba algo en el puerto serial o en el Bluetooth
  while (bluetooth.available() == 0) {
    // Espera aquí hasta que se reciba algo en cualquiera de los puertos
  }

  // Verifica si se recibió un dato desde el Bluetooth
  if (bluetooth.available() > 0) {
    char opcion = bluetooth.read();
    if (opcion == '1') {
      digitalWrite(pin13, HIGH);  // ENCENDER PIN 13
      // MOSTRAR "LUZ ENCENDIDA" POR BLUETOOTH
      // MOSTRAR "LUZ ENCENDIDA" POR MONITOR SERIAL
    } else if (opcion == '0') {
      // APAGAR PIN 13
      // MOSTRAR "LUZ APAGADA" POR BLUETOOTH
      // MOSTRAR "LUZ APAGADA" POR MONITOR SERIAL
    }

    delay(2000);
  }

  // Limpiar el buffer serial antes de continuar
  while (bluetooth.available() > 0) {
    bluetooth.read();  // Descartar cualquier carácter residual en el buffer
  }
}
