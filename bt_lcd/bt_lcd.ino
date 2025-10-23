// Variable para almacenar el nombre
String nombre = "";
const int led = 13;

void setup() {
  pinMode(led, OUTPUT);
  // Inicializa la comunicación serial a 9600 baudios para el módulo Bluetooth
  Serial.begin(9600);

  // Mensaje de bienvenida en el monitor serial
  Serial.println("Bienvenido!");

  delay(2000); // Espera 2 segundos
}

void loop() {
  // Mensaje pidiendo el nombre
  Serial.println("Ingrese su nombre: ");
  delay(1000);

  // Espera a que el usuario ingrese un nombre
  while (Serial.available() == 0) {
    // Espera aquí hasta que se reciba algo
  }

  // Verifica si hay datos disponibles en el puerto serial
  if (Serial.available() > 0) {
    // Lee los datos ingresados hasta el salto de línea
    nombre = Serial.readStringUntil('\n');

    // Elimina cualquier retorno de carro (\r) del nombre
    nombre.trim();

    // Mostrar el nombre ingresado en el monitor serial
    Serial.print("Nombre: ");
    Serial.println(nombre);

    // Espera un tiempo para que el usuario vea el nombre
    delay(5000);
  }

  Serial.println("Ingrese 1 para encender led");
  Serial.println("Ingrese 0 para apagar led");



  while (Serial.available() == 0) {
    // Espera aquí hasta que se reciba algo
  }

  if (Serial.available() > 0) {
    char opcion = Serial.read();
    if (opcion == '1') {
      digitalWrite(led, HIGH);
      Serial.println("LED encendido");
    } else if (opcion == '0') {
      digitalWrite(led, LOW);
      Serial.println("LED apagado");
    }

    delay(2000);
  }

    // Limpiar el buffer serial antes de continuar
  while (Serial.available() > 0) {
    Serial.read();  // Descartar cualquier carácter residual en el buffer
  }
}
