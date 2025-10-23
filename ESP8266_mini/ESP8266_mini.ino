void setup() {
  pinMode(2, OUTPUT);  // GPIO2
}

void loop() {
  digitalWrite(2, LOW);   // Encender LED
  delay(500);
  digitalWrite(2, HIGH);  // Apagar LED
  delay(500);
}
