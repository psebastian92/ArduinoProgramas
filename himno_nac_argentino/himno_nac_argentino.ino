// Pines y duraciones de las notas
const int buzzer = 8; // Cambia al pin donde tienes conectado el buzzer

// Notas musicales (frecuencia en Hz)
#define C4  262
#define D4  294
#define E4  330
#define F4  349
#define G4  392
#define A4  440
#define B4  494
#define C5  523
#define D5  587
#define E5  659
#define F5  698
#define G5  784

// Melodía del estribillo del Himno Nacional Argentino
int melody[] = {
  E4, G4, A4, A4, G4, A4, B4, B4, A4, B4, C5, B4, A4, G4,
  E4, E4, E4, G4, F4, E4, D4, C4, C4
};

// Duración de las notas (4=croche, 8=fusa, etc.)
int noteDurations[] = {
  8, 8, 4, 8, 8, 8, 4, 8, 8, 8, 4, 8, 8, 8,
  4, 8, 8, 4, 8, 8, 8, 4, 2
};

void setup() {

}

void loop() {
   // Reproducción de la melodía
  for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(melody[0]); thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzer, melody[thisNote], noteDuration);

    // Pausa entre notas
    int pauseBetweenNotes = noteDuration * 1.3;
    delay(pauseBetweenNotes);
    
    // Apagar buzzer después de cada nota
    noTone(buzzer);
  }
}
