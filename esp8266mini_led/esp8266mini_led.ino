#define LED_ROJO D5  // D5 en NodeMCU/Wemos
#define LED_VERDE D6 // D6 en NodeMCU/Wemos

void setup() {
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
}

void loop() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_VERDE, HIGH);
    delay(500);  // medio segundo encendidos

    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_VERDE, LOW);
    delay(500);  // medio segundo apagados
  }

  // Después de las 5 veces, se quedan apagados
  while (true);  // detener el loop
}
