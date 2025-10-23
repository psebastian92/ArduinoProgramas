#include <ESP8266WiFi.h> // Librería para vincular ESP8266 a WiFi

const int pinDigital = 2; // GPIO 2 -> D4 en esp8266
const int pinPulsador = 0; // GPIO 0 -> D3 en esp8266

void setup() {
  Serial.begin(9600);
  pinMode(pinDigital, OUTPUT);
  pinMode(pinPulsador, INPUT);
}

void loop() {
  if (digitalRead(pinPulsador) == LOW ) {
    Serial.println("Pulsador ON");
    digitalWrite(pinDigital, HIGH);
    Serial.println(digitalRead(pinPulsador));
    delay(35000);
    digitalWrite(pinDigital, LOW);
    delay(5000);
  }
  else {
    Serial.println("Pulsador OFF");
    delay(1000);
  }
}
