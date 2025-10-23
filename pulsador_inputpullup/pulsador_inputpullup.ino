// Definición de constantes para los pines, mejorando la legibilidad.
const int pinBoton = 7;
const int pinLed = 13;

void setup() {
  Serial.begin(9600); // Inicia la comunicación serial.
  
  // Configura el pin del botón como entrada con resistencia PULLUP.
  // Esto hace que el pin esté HIGH por defecto y LOW al presionar el botón.
  pinMode(pinBoton, INPUT_PULLUP);
  
  // Configura el pin del LED como salida.
  pinMode(pinLed, OUTPUT);
}

void loop() {
  // Lee el estado del botón.
  int estadoBoton = digitalRead(pinBoton);
  
  // Muestra el estado del botón en el Monitor Serial.
  Serial.println(estadoBoton);

  // Enciende o apaga el LED basándose en el estado del botón.
  // Con INPUT_PULLUP, el botón se activa con un valor LOW.
  if (estadoBoton == LOW) {
    digitalWrite(pinLed, HIGH); // Enciende el LED cuando el botón está presionado.
  } else {
    digitalWrite(pinLed, LOW);  // Apaga el LED cuando el botón no está presionado.
  }
  
  delay(100); // Un pequeño retraso para evitar lecturas inestables (debounce).
}