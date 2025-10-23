
/*

  digitalWrite(encendidoLCD, LOW) -> enciende el pin conectado al LCD


  digitalWrite(encendidoLCD, HIGH) -> apaga el pin conectado al LCD (por ende, apaga el LCD)


  lcd.init() -> enciende lcd
  lcd.backlight() -> Enciende la iluminación de fondo
  lcd.setCursor(columna, fila) -> Setea desde dónde va a escribir. Ejemplo lcd.setCursor(0, 0) (arriba a la izquierda)
  lcd.print("Texto a mostrar") -> Muestra el texto entre comillas
  lcd.print(variableAMostrar) -> Muestra el valor de una variable (fíjense que no hay comillas acá)
  lcd.clear() -> limpia la pantalla


*/


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>

WiFiClient client;

// CONSTANTES PARA CONECTARSE A WIFI
const char *ssid = "Sebas Cel";
const char *password = "";
const char *serverAddress = "http://humedaddelsuelo.000webhostapp.com/recibirDatos.php";

// CONSTANTES PARA INDICAR NÚMERO DE FILAS Y COLUMNAS DEL LCD
const int lcdColumns = 16;
const int lcdRows = 2;

const int encendidoLCD = D5;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// VARIABLES ENTERAS NECESARIAS. SE INICIALIZAN EN 0.
int sensor = 0;
int porcentajeHumedad = 0;
int intentosWiFi = 0;
int intentosEncendido = 9;
int intentosApagado = 9;
int pinPulsador = D7;
/*----------------------------------------*/
/*-----------VOID SETUP-------------------*/
/*----------------------------------------*/
void setup() {
  Serial.begin(9600);
  pinMode(encendidoLCD, OUTPUT);
  pinMode(pinPulsador, INPUT);

  digitalWrite(encendidoLCD, LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.clear();

  lcd.print("APRETAR BOTON!");

  while (intentosEncendido >= 0) {
    // CORREGIR  lcd.setCursor(0, 0);
    // CORREGIR  lcd.print("Apretar boton: ");
    lcd.setCursor(0, 1);
    lcd.print(intentosEncendido);
    intentosEncendido--;
    delay(1000);

    if ( digitalRead(pinPulsador) == LOW ) { // PULSADOR APRETADO

      digitalWrite(encendidoLCD, LOW);
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print("Encendiendo lcd");
      delay(2000);
      break;
    }

    if (intentosEncendido == 0) {
      lcd.setCursor (0, 0);
      lcd.clear();
      lcd.print ("Apagando LCD");
      delay(2000);
      digitalWrite (encendidoLCD, HIGH);


    }

  }



  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && intentosWiFi <= 10) {
    lcd.clear();
    lcd.print("Intentos WiFi:");
    lcd.setCursor(0, 1);
    lcd.print (intentosWiFi);
    intentosWiFi++;
    delay(1000);
  }

  if (intentosWiFi < 10) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Conectado a WiFi");
    delay(2000);
    lcd.clear();
  } else {
    lcd.clear();
    lcd.print("No hay conexion");
    lcd.setCursor(0, 1);
    lcd.print("WiFi");
    delay(2000);
    lcd.clear();
  }


  lcd.setCursor(0, 0);
  lcd.print("Bienvenido!");
  // lcd.setCursor(0, 1);
  // lcd.print("Petruko");
  delay(3000);
  lcd.clear();


}

/*----------------------------------------*/
/*-----------VOID LOOP-------------------*/
/*----------------------------------------*/
void loop() {
  String mensajeHumedad;
  sensor = analogRead(A0);
  porcentajeHumedad = (sensor - 1024) / (-10.24);

  if (porcentajeHumedad < 40) {
    mensajeHumedad = "Regar!";
  } else {
    mensajeHumedad = "Tierra Húmeda";
  }

  while (intentosEncendido >= 0 && intentosApagado > 0) {
    // SI NO ANALIZO ESTAS DOS CONDICIONES, NO FUNCIONA.
    if (digitalRead(pinPulsador) == LOW) { // PULSADOR APRETADO
      digitalWrite(encendidoLCD, LOW);  // ENCENDIENDO EL PIN DEL RELÉ DEL LCD
      delay(500);
      lcd.init();
      lcd.backlight();

    }
    intentosEncendido--;
  }

  intentosApagado = 9;
  intentosEncendido = 9;

  if ( digitalRead(encendidoLCD) == LOW ) {
    lcd.setCursor(0, 0);
    lcd.print("Humedad: ");
    lcd.setCursor(9, 0);
    lcd.print(porcentajeHumedad);
    //  lcd.setCursor(0, 1);
    //lcd.print("<3");
    delay(60000);
    lcd.clear();
    lcd.print("Presiona para");
    lcd.setCursor(0, 1);
    lcd.print("apagar");
  }
  else {
    delay(60000);
  }



  while (intentosApagado >= 0) {
    lcd.setCursor(7, 1);
    lcd.print(intentosApagado);
    intentosApagado--;
    delay(1000);

    if ( digitalRead(pinPulsador) == LOW) { // PULSADOR APRETADO

      digitalWrite(encendidoLCD, HIGH);
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print("Apagando... ");
      delay(2000);
      break;
    }
  }


  if (WiFi.status() != WL_CONNECTED) {
    lcd.clear();
    lcd.print("Reconectando");
    WiFi.begin(ssid, password);
    delay(3000);
  }
  lcd.clear();


  enviarDatosAlServidor(mensajeHumedad, porcentajeHumedad);

}

void enviarDatosAlServidor(String mensajeHumedad, int porcentajeHumedad) {
  HTTPClient http;
  http.begin(client, serverAddress);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String enviarDatos = "mensajeHumedad=" + mensajeHumedad + "&porcentajeHumedad=" + String(porcentajeHumedad);

  int httpCodigoRespuesta = http.POST(enviarDatos);
  Serial.println(httpCodigoRespuesta);
  if (httpCodigoRespuesta > 0) {
    Serial.print("HTTP Código respuesta: ");
    Serial.println(httpCodigoRespuesta);
    if (httpCodigoRespuesta == 200) {
      String cuerpo_respuesta = http.getString();
      Serial.println("El servidor respondió: ");
      Serial.println(cuerpo_respuesta);
    }
  } else {
    Serial.print("Error en la solicitud: ");
    Serial.println(http.errorToString(httpCodigoRespuesta));
  }

  http.end();
}
