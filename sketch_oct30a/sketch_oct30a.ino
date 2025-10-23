#include <LiquidCrystal_I2C.h> 
int sensor = 0;
int humedad=0;
// Define el número de columnas y filas del LCD
const int lcdColumns = 16;
const int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
void setup() {
  Serial.begin(9600); // conecta la pc con los datos del microcontrolador
   lcd.init();

  // Enciende la retroiluminación
  lcd.backlight();

  // (0,0) es la columna 0 y la fila 0 (arriba a la izquierda)
  lcd.setCursor(0, 0);


  //Dentro de las comillas se coloca una frase que saldrá en
  // pantalla. Tener en cuenta que hay máximo 16 caracteres.
  // Colocar una frase de bienvenida.
  lcd.print("bienvenido");
  delay(2000);
  lcd.clear(); ;
}
 
void loop() {

lcd.print("bienvenido");
delay(3000);
lcd.clear();
sensor = analogRead(0);
humedad = (sensor-1024)/(-10.24);

 lcd.setCursor(0, 1);
 lcd.print(humedad);
 Serial.println("%");
 delay(3000);
 lcd.clear();
Serial.print(humedad);
delay(500);



}
