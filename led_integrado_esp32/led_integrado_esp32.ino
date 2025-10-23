// Definimos el pin donde está conectado el LED
#define LED_PIN 2  // En el ESP32, el pin GPIO 2 suele tener un LED integrado

void setup() {
  // Configuramos el pin como salida
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Encendemos el LED
  digitalWrite(LED_PIN, HIGH);
  delay(1000); // Esperamos 1 segundo (1000 ms)

  // Apagamos el LED
  digitalWrite(LED_PIN, LOW);
  delay(1000); // Esperamos 1 segundo (1000 ms)
}
