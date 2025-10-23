
#define led_rojo 11
#define led_verde 12
int sensor=0, contador=0;
int valor = 0;  
void setup() { 
  Serial.begin(9600);
  
  pinMode(led_rojo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(sensor,INPUT);
} 
void loop() {
  sensor = analogRead(A0);
  Serial.print(sensor);
  Serial.print("\n");
  if(sensor>200){ //CALIBRAR EL SENSOR
    digitalWrite(led_rojo,HIGH);
    delay(500);
    digitalWrite(led_rojo,LOW);
    contador++;
  }
  if(contador==5){
    digitalWrite(led_verde,HIGH);
    delay (7000);
    digitalWrite (led_verde,LOW);
    contador = 0;
  }
  
  delay(100);
}
