#define pinMedicion 0    // Conectar el sensor de polvo al pin A0 del Arduino
#define pinLed 13        // Conectar 3 pines del controlador del LED del sensor de polvo al pin D13 del Arduino
int tiempoMuestreo = 280; // Tiempo necesario para muestrear la señal que sale del sensor
int tiempoDelta = 40;     //
int tiempoDormir = 9680;  //
float voltajeMedido = 0;
float voltajeCalculado = 0;
float densidadPolvo = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
}

void loop() {
  digitalWrite(pinLed, LOW); // Encender el LED
  delayMicroseconds(tiempoMuestreo);
  voltajeMedido = analogRead(pinMedicion); // Leer el valor de polvo
  delayMicroseconds(tiempoDelta);
  digitalWrite(pinLed, HIGH); // Apagar el LED
  delayMicroseconds(tiempoDormir);

  voltajeCalculado = voltajeMedido * (5.0 / 1024.0);

  densidadPolvo = 170 * voltajeCalculado - 0.1;
  
  // Imprimir valores para depuración
  Serial.print("Voltaje Medido: ");
  Serial.println(voltajeCalculado);
  Serial.print("Densidad de Polvo: ");
  Serial.println(densidadPolvo);

  delay(1000);
}
