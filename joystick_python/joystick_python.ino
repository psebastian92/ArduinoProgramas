const int joyX = A0;  // Pin del eje X del joystick
const int joyY = A1;  // Pin del eje Y del joystick
const int buttonSW = 9;  // Pin del botón SW del joystick

void setup() {
  Serial.begin(9600);  // Inicializa la comunicación serial
  pinMode(buttonSW, INPUT_PULLUP);  // Configura el botón SW como entrada con resistencia de pull-up
}

void loop() {
  int xValue = analogRead(joyX);  // Lee el valor del eje X
  int yValue = analogRead(joyY);  // Lee el valor del eje Y
  int buttonState = digitalRead(buttonSW);  // Lee el estado del botón SW
  
  // Envía los valores al puerto serie
  Serial.print(xValue);
  Serial.print(",");
  Serial.print(yValue);
  Serial.print(",");
  Serial.println(buttonState == LOW ? 1 : 0);  // Envía 1 si el botón está presionado, 0 si no
  delay(100);  // Espera 100 ms antes de la próxima lectura
}
