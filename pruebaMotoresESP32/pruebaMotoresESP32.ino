#define IN7 32  // Motor 2 - Izquierdo trasero
#define IN8 18

void setup() {
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
}

void loop() {
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);  // Motor 2
  delay(1000);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);  // Motor 2
  delay(1000);
}
