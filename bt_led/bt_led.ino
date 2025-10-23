#include <SoftwareSerial.h>

// Configura el pin 2 como RX y el pin 3 como TX para el Bluetooth
SoftwareSerial bluetooth(2, 3); // RX, TX

String nombre = "";
const int led = 13;

void setup() {
  pinMode(led, OUTPUT);

  // Inicializa la comunicación serial en los pines 0 y 1 para la computadora
  Serial.begin(9600);
  
  // Inicializa la comunicación serial en los pines 2 y 3 para el Bluetooth
  bluetooth.begin(9600);

  // Mensaje de bienvenida en ambos
  Serial.println("Bienvenido!");
  bluetooth.println("Bienvenido!");

  delay(2000); // Espera 2 segundos
}

void loop() {
  // Mensaje pidiendo el nombre
  Serial.println("Ingrese su nombre: ");
  bluetooth.println("Ingrese su nombre: ");
  delay(1000);

  // Espera a que el usuario ingrese un nombre desde el Bluetooth
  while (bluetooth.available() == 0) {
    // Espera aquí hasta que se reciba algo
  }

  // Verifica si hay datos disponibles en el puerto serial del Bluetooth
  if (bluetooth.available() > 0) {
    // Lee los datos ingresados hasta el salto de línea
    nombre = bluetooth.readStringUntil('\n');

    // Elimina cualquier retorno de carro (\r) del nombre
    nombre.trim();

    // Mostrar el nombre ingresado en el monitor serial y en la aplicación Bluetooth
    Serial.print("Nombre: ");
    Serial.println(nombre);
    bluetooth.print("Nombre: ");
    bluetooth.println(nombre);

    // Espera un tiempo para que el usuario vea el nombre
    delay(5000);
  }

  Serial.println("Ingrese 1 para encender led");
  bluetooth.println("Ingrese 1 para encender led");
  Serial.println("Ingrese 0 para apagar led");
  bluetooth.println("Ingrese 0 para apagar led");

  while (bluetooth.available() == 0) {
    // Espera aquí hasta que se reciba algo
  }

  if (bluetooth.available() > 0) {
    char opcion = bluetooth.read();
    if (opcion == '1') {
      digitalWrite(led, HIGH);
      Serial.println("LED encendido");
      bluetooth.println("LED encendido");
    } else if (opcion == '0') {
      digitalWrite(led, LOW);
      Serial.println("LED apagado");
      bluetooth.println("LED apagado");
    }

    delay(2000);
  }

  // Limpiar el buffer serial antes de continuar
  while (bluetooth.available() > 0) {
    bluetooth.read();  // Descartar cualquier carácter residual en el buffer
  }
}
