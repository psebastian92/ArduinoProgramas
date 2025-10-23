// Definimos los pines para el LED RGB
const int redPin = 11;    // Pin para el LED rojo
const int greenPin = 12;  // Pin para el LED verde
const int bluePin = 13;   // Pin para el LED azul

void setup() {
  // Configuramos los pines como salidas
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // Cambiamos el color del LED RGB a diferentes combinaciones
  setColor(255, 0, 0); // Rojo
  delay(1000);
  setColor(0, 255, 0); // Verde
  delay(1000);
  setColor(0, 0, 255); // Azul
  delay(1000);
  setColor(255, 255, 0); // Amarillo
  delay(1000);
  setColor(0, 255, 255); // Cian
  delay(1000);
  setColor(255, 0, 255); // Magenta
  delay(1000);
  setColor(255, 255, 255); // Blanco
  delay(1000);
  setColor(0, 0, 0); // Apagar LED
  delay(1000);
}

// Función para establecer el color del LED RGB
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);      // Escribimos el valor de rojo
  analogWrite(greenPin, green);  // Escribimos el valor de verde
  analogWrite(bluePin, blue);    // Escribimos el valor de azul
}
