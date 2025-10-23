// Pines del joystick
const int pinX = A0;
const int pinY = A1;

// Pines de los LEDs
const int led1 = 3;
const int led2 = 5;
const int led3 = 6;
const int led4 = 11;
const int pulsadorJoystick = 9;

// Constantes
const int zonaMuerta = 50; // Ajusta esta zona muerta según sea necesario

void setup() {
  // Configuramos los pines de los LEDs como salida
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(pulsadorJoystick, INPUT);
}
void loop() {
  int valorX = 0;
 // int Yvalue = 0;
  bool buttonValue = false;
  buttonValue = digitalRead(pulsadorJoystick);
  valorX = analogRead(pinX);
  valorX = valorX / 4;
  analogWrite(led1, valorX);
  Serial.println(valorX);


  if (buttonValue==true) {
    Serial.println("Se presiono el stick");

    for (int i = 0; i < 3; i++) {
      digitalWrite(led1, HIGH);
      delay(500);
      digitalWrite(led1, LOW);
    }


  }
}
