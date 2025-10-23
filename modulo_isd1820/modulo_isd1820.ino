#define TRIG_PIN 7       // Pin para TRIG del ultrasonido
#define ECHO_PIN 6       // Pin para ECHO del ultrasonido
#define PLAY_PIN 2       // Pin para activar reproducción en ISD1820

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PLAY_PIN, OUTPUT);
  
  digitalWrite(PLAY_PIN, HIGH);  // Inicializamos PLAY_PIN en HIGH (inactivo)
  Serial.begin(9600);            // Para depurar en el monitor serie
}

void loop() {
  // Enviamos un pulso de 10µs al TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Leemos el tiempo de ida y vuelta del pulso
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculamos la distancia en cm
  int distance = duration * 0.034 / 2;
  
  // Imprimimos la distancia en el monitor serie
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Verificamos si la distancia es menor a 30 cm
  if (distance > 0 && distance < 30) {
    // Activamos el PLAY_PIN para reproducir el mensaje
    digitalWrite(PLAY_PIN, LOW);   // Inicia reproducción
    delay(1000);                   // Reproducción por 1 segundo
    digitalWrite(PLAY_PIN, HIGH);  // Detiene reproducción
    delay(2000);                   // Espera antes de medir de nuevo
  }
  
  delay(100);  // Pequeño retardo para evitar lecturas muy rápidas
}
