// Definir el pin del sensor MQ-2
const int gasPin = A0;

// Variable para almacenar el valor del sensor MQ-2
int gasValue;

void setup() {
  // Inicializar el pin del sensor
  pinMode(gasPin, INPUT);

  // Inicializar la comunicación serial
  Serial.begin(115200);
}

void loop() {
  // Leer el valor del sensor MQ-2
  gasValue = analogRead(gasPin);

  // Mostrar el valor del sensor de gas en el monitor serial
  Serial.print("Valor de gas: ");
  Serial.println(gasValue);

  // Esperar un breve momento antes de la próxima medición
  delay(1000);
}
