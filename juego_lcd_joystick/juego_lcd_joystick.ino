#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Cambia 0x27 por 0x3F si tu módulo I2C usa esa dirección
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pines del joystick
const int JOY_X = A0;     // (no usado en este juego, pero lo dejamos por si quieres extenderlo)
const int JOY_Y = A1;
const int BTN   = 2;      // botón con INPUT_PULLUP

// Umbrales del joystick (vertical)
const int DEAD_LOW  = 450;
const int DEAD_HIGH = 570;

// Juego
const byte LCD_COLS = 20;
const byte LCD_ROWS = 4;

enum GameState { MENU, PLAYING, GAME_OVER };
GameState state = MENU;

struct Obstacle {
  int col;
  byte row;
  bool active;
};

const byte MAX_OBS = 6;
Obstacle obs[MAX_OBS];

byte playerRow = 1;        // empieza en fila 1 (0..3)
unsigned long lastFrame = 0;
unsigned long frameInterval = 110;   // ms por frame (ajusta para más/menos velocidad)
unsigned long lastSpawn = 0;
unsigned long spawnInterval = 600;   // ms entre spawns (se reduce con la puntuación)

unsigned long score = 0;
unsigned long bestScore = 0;

bool lastBtn = HIGH;

// Custom chars
byte playerChar[8]   = {B00100,B00110,B11111,B11111,B11111,B00110,B00100,B00000}; // especie de flecha/diamante
byte obstacleChar[8] = {B00000,B11111,B10101,B11111,B10101,B11111,B00000,B00000}; // bloque

void resetGame() {
  // Limpia obstáculos
  for (byte i = 0; i < MAX_OBS; i++) {
    obs[i].active = false;
  }
  playerRow = 1;
  score = 0;
  frameInterval = 110;
  spawnInterval = 600;
  lastFrame = millis();
  lastSpawn = millis();
}

void spawnObstacle() {
  // Busca un slot libre
  for (byte i = 0; i < MAX_OBS; i++) {
    if (!obs[i].active) {
      obs[i].active = true;
      obs[i].col = LCD_COLS - 1;              // aparece en la última columna (19)
      obs[i].row = random(0, LCD_ROWS);       // fila aleatoria 0..3
      return;
    }
  }
}

void updateObstacles() {
  for (byte i = 0; i < MAX_OBS; i++) {
    if (obs[i].active) {
      obs[i].col--;
      if (obs[i].col < 0) {
        obs[i].active = false;   // salió de la pantalla
        score++;
        // Aumenta dificultad levemente
        if (spawnInterval > 280) spawnInterval -= 5;
        if (frameInterval > 70) frameInterval -= 1;
      }
    }
  }
}

bool checkCollision() {
  for (byte i = 0; i < MAX_OBS; i++) {
    if (obs[i].active && obs[i].col == 0 && obs[i].row == playerRow) {
      return true;
    }
  }
  return false;
}

void drawFrame() {
  lcd.clear();

  // Dibuja obstáculos
  for (byte i = 0; i < MAX_OBS; i++) {
    if (obs[i].active && obs[i].col >= 0 && obs[i].col < LCD_COLS) {
      lcd.setCursor(obs[i].col, obs[i].row);
      lcd.write(byte(1)); // obstáculo
    }
  }

  // Dibuja jugador
  lcd.setCursor(0, playerRow);
  lcd.write(byte(0));

  // HUD arriba
  lcd.setCursor(6, 0);
  lcd.print("Score:");
  lcd.print(score);
  lcd.setCursor(6, 1);
  lcd.print("Best:");
  lcd.print(bestScore);
}

void readInput() {
  int y = analogRead(JOY_Y);

  // Movimiento por "paso" con zona muerta
  if (y < DEAD_LOW && playerRow > 0) {
    playerRow--;
    delay(60); // antirebote de movimiento para que no salte 2 filas de golpe
  } else if (y > DEAD_HIGH && playerRow < (LCD_ROWS - 1)) {
    playerRow++;
    delay(60);
  }
}

bool buttonPressed() {
  bool b = digitalRead(BTN);
  bool pressed = (lastBtn == HIGH && b == LOW); // flanco descendente (porque PULLUP)
  lastBtn = b;
  return pressed;
}

void setup() {
  pinMode(BTN, INPUT_PULLUP);
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, playerChar);
  lcd.createChar(1, obstacleChar);

  // Semilla aleatoria (si tienes un pin flotante libre, mejor)
  randomSeed(analogRead(A3));

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("LCD Runner 20x4");
  lcd.setCursor(1, 2);
  lcd.print("Pulsa el boton");
  lcd.setCursor(1, 3);
  lcd.print("para comenzar");
}

void loop() {
  switch (state) {
    case MENU:
      if (buttonPressed()) {
        resetGame();
        state = PLAYING;
      }
      break;

    case PLAYING: {
      // Entrada
      readInput();

      unsigned long now = millis();

      // Spawns
      if (now - lastSpawn >= spawnInterval) {
        spawnObstacle();
        lastSpawn = now;
      }

      // Avance de juego
      if (now - lastFrame >= frameInterval) {
        updateObstacles();
        if (checkCollision()) {
          state = GAME_OVER;
          if (score > bestScore) bestScore = score;
          lcd.clear();
          lcd.setCursor(4, 1);
          lcd.print("GAME OVER :(");
          lcd.setCursor(3, 2);
          lcd.print("Score: ");
          lcd.print(score);
          lcd.setCursor(3, 3);
          lcd.print("Best : ");
          lcd.print(bestScore);
        } else {
          drawFrame();
        }
        lastFrame = now;
      }

      // Permite salir/reiniciar
      if (buttonPressed()) {
        resetGame();
      }
    } break;

    case GAME_OVER:
      // Espera botón para volver a menú o reiniciar directo
      if (buttonPressed()) {
        state = MENU;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("LCD Runner 20x4");
        lcd.setCursor(1, 2);
        lcd.print("Pulsa el boton");
        lcd.setCursor(1, 3);
        lcd.print("para comenzar");
      }
      break;
  }
}
