
int seq[100];
int rodada = 0;
int paso = 0;
int leds[3] = { 9, 10, 11 };
int botones[3] = { 2, 3, 4 };
int boton_presionado = 0;
int perder = false;
int buzzer = 6;
int sonidos[4]={400, 450, 500, 550};

void setup() {
  pinMode(buzzer,OUTPUT);
  tone(buzzer,sonidos[2]);
  delay(20);
  noTone(buzzer);
  for (int i = 0; i <= 2; i++) {
    pinMode(leds[i], OUTPUT);
     }

  for (int i = 0; i <= 2; i++) {
    pinMode(botones[i], INPUT);
  }
  randomSeed(analogRead(0));

}

void loop() {

  if (perder) {
    int seq[100];
    rodada = 0;
    paso = 0;
    perder = false;
  }

  if (rodada == 0) {
    digitalWrite(leds[0], HIGH);
    digitalWrite(leds[1], HIGH);
    digitalWrite(leds[2], HIGH);
 
    delay(424);
    digitalWrite(leds[0], LOW);
    digitalWrite(leds[1], LOW);
    digitalWrite(leds[2], LOW);
  
    delay(242);
  }

  seq[rodada++] = random(0, 3);

  for (int i = 0; i < rodada; i++) {
    digitalWrite(leds[seq[i]], HIGH);
    tone(buzzer,sonidos[seq[i]]);
    delay(500);
    digitalWrite(leds[seq[i]], LOW);
    noTone(buzzer);
    delay(100);
  }

  for (int i = 0; i < rodada; i++) {

    boolean jugada_efectuada = false;
    while (!jugada_efectuada) {
      for (int i = 0; i <= 2; i++) {
        if (digitalRead(botones[i]) == HIGH) {
          boton_presionado = i;
          digitalWrite(leds[i], HIGH);
          delay(300);
          digitalWrite(leds[i], LOW);
          jugada_efectuada = true;
        }
      }
      delay(10);
    }

    if (seq[paso] != boton_presionado) {

      for (int i = 0; i <= 2; i++) {

        digitalWrite(leds[i], HIGH);
        delay(200);
        digitalWrite(leds[i], LOW);
      }

      for (int i = 0; i <= 2; i++) {

        digitalWrite(leds[0], HIGH);
        digitalWrite(leds[1], HIGH);
        digitalWrite(leds[2], HIGH);
   
        delay(100);
        digitalWrite(leds[0], LOW);
        digitalWrite(leds[1], LOW);
        digitalWrite(leds[2], LOW);
      
        delay(100);
      }

      perder = true;
      tone(buzzer,100);
      delay(100);
      noTone(buzzer);
    }

    if (perder) {
      break;
    }

    paso++;
  }

  paso = 0;

  delay(1000);
}
