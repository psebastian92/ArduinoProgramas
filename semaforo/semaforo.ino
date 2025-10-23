const int pinX = A0;
const int pinY = A1;
const int pinSW = 9;

void setup() {
  Serial.begin(9600);
  pinMode(pinSW, INPUT_PULLUP);  // Configurar el pin SW como entrada con resistencia pull-up
}

void loop() {
  int valorX = analogRead(pinX);
  int valorY = analogRead(pinY);
  int buttonSW = digitalRead(pinSW);  // Leer el estado del botón SW

  Serial.print(valorX);
  Serial.print(",");
  Serial.print(valorY);
  Serial.print(",");
  Serial.println(buttonSW);
  
  delay(100);  // Retraso para la estabilidad de la lectura
}
