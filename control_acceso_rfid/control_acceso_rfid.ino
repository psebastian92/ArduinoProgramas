// Llavero: C3 DB 94 18
// Tarjeta: C3 8C BA 06

#include <SPI.h>      // incluye libreria bus SPI
#include <MFRC522.h>      // incluye libreria especifica para MFRC522

#define RST_PIN  9      // constante para referenciar pin de reset
#define SS_PIN  10      // constante para referenciar pin de slave select
const int pinRele = 7;

MFRC522 mfrc522(SS_PIN, RST_PIN); // crea objeto mfrc522 enviando pines de slave select y reset

byte LecturaUID[4];         // crea array para almacenar el UID leido
byte Usuario1[4] = {0xC3, 0xDB, 0x94, 0x18} ;   // UID de llavero leido en programa 1
byte Usuario2[4] = {0xC3, 0x8C, 0xBA, 0x06} ;   // UID de tarjeta leido en programa 1

void setup() {
  pinMode(pinRele, OUTPUT);
  digitalWrite(pinRele, HIGH); // Asegurarse de que el relé esté apagado al inicio (recordar que se apaga con nivel alto)
  delay(5000); //espera de 5000ms (5 segundos)
  Serial.begin(9600);     // inicializa comunicacion por monitor serie a 9600 bps
  SPI.begin();        // inicializa bus SPI
  mfrc522.PCD_Init();     // inicializa modulo lector
  Serial.println("Comenzando");    // Muestra texto "Comenzando"
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {  // si no hay una tarjeta presente
    return;           // retorna al loop esperando por una tarjeta
  }
  if ( ! mfrc522.PICC_ReadCardSerial())  {   // si no puede obtener datos de la tarjeta
    return;           // retorna al loop esperando por otra tarjeta
  }
  Serial.print("UID:");       // muestra texto UID:
  for (byte i = 0; i < mfrc522.uid.size; i++) { // bucle recorre de a un byte por vez el UID
    if (mfrc522.uid.uidByte[i] < 0x10) {  // si el byte leido es menor a 0x10
      Serial.print(" 0");       // imprime espacio en blanco y numero cero
    }
    else {          // sino
      Serial.print(" ");        // imprime un espacio en blanco
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX);    // imprime el byte del UID leido en hexadecimal
    LecturaUID[i] = mfrc522.uid.uidByte[i];   // almacena en array el byte del UID leido
  }

  Serial.print("\t");         // imprime un espacio de tabulacion

  if (comparaUID(LecturaUID, Usuario1)) {  // llama a funcion comparaUID con Usuario1
    Serial.println("Bienvenido Usuario 1"); // si retorna verdadero muestra texto bienvenida
    digitalWrite(pinRele, LOW); // activamos el rele mediante el pin 7 (recordar que se enciende con nivel bajo)
    delay(5000); // esperamos 5000ms (5 segundos)
    digitalWrite(pinRele, HIGH); //(recordar que se apaga con nivel alto)
  }
  else if (comparaUID(LecturaUID, Usuario2)) { // llama a funcion comparaUID con Usuario2
    Serial.println("Bienvenido Usuario 2"); // si retorna verdadero muestra texto bienvenida
    digitalWrite(pinRele, LOW); // activamos el rele mediante el pin 7 (recordar que se enciende con nivel bajo)
    delay(5000); // esperamos 5000ms (5 segundos)
    digitalWrite(pinRele, HIGH); //(recordar que se apaga con nivel alto)
  }
  else     {      // si retorna falso
    Serial.println("Acceso denegado");    // muestra texto equivalente a acceso denegado
    digitalWrite(pinRele, HIGH); // desactivamos el rele mediante el pin 7 (recordar que se apaga con nivel alto)
  }
  mfrc522.PICC_HaltA();     // detiene comunicacion con tarjeta
}

boolean comparaUID(byte lectura[], byte usuario[]) // funcion comparaUID
{
  for (byte i = 0; i < mfrc522.uid.size; i++) { // bucle recorre de a un byte por vez el UID
    if (lectura[i] != usuario[i])       // si byte de UID leido es distinto a usuario
      return (false);         // retorna falso
  }
  return (true);          // si los 4 bytes coinciden retorna verdadero
}
