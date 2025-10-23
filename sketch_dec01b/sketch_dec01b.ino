int leds[3] = { 9, 10, 11 };
int botones[3] = { 2, 4, 7 };

void setup()
{
   for (int i = 0; i <= 2; i++) {
    pinMode(leds[i], OUTPUT);
     }

  for (int i = 0; i <= 2; i++) {
    pinMode(botones[i], INPUT);
  }
}

void loop()
{
  if ( digitalRead (2) == HIGH){
    digitalWrite(9,HIGH);
  }
  else {
   digitalWrite(9,LOW);
  }

  if ( digitalRead (7) == HIGH){
    digitalWrite(10,HIGH);
  }
  else {
   digitalWrite(10,LOW);
  }

  if ( digitalRead (4) == HIGH){
    digitalWrite(11,HIGH);
  }
  else {
   digitalWrite(11,LOW);
  }

  
  }
