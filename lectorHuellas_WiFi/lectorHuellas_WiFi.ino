#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>  // Comunicación serial por software
#include <ESP8266WiFi.h>      // Librería para ESP8266 (para WiFi)
#include <ESP8266HTTPClient.h> // Librería para HTTP

#define RELE 2         // Pin del relé. ES EL PIN D4
#define BUTTON_PIN 14  // Pin del pulsador para registro de nuevas huellas. ES EL PIN D5

// Definición de objetos
SoftwareSerial miSerial(12, 13);  // Pines para el sensor de huellas
Adafruit_Fingerprint huellaSensor = Adafruit_Fingerprint(&miSerial);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Configuración del LCD I2C

// Variables globales
bool EncenderRele = true;       // Para controlar el encendido/apagado del relé
unsigned long tiempoBoton = 0;  // Variable para almacenar el tiempo en que se pulsa el botón
int id;
const char* ssid = "TP-Link_3FE2";    // Cambia con el nombre de tu red Wi-Fi
const char* password = "26826615"; // Cambia con la contraseña de tu red Wi-Fi
const char* serverUrl = "http://192.168.0.109:8080/RecibirIDLectorHuella/RecibirDatosESP8266"; // URL del servidor

void setup() {
  // Inicializa la comunicación serial y el LCD
  inicializarLCD();
  inicializarSensorHuellas();

  // Configura los pines del relé y el pulsador
  pinMode(RELE, OUTPUT);              // Configura el pin del relé como salida
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Configura el pin del pulsador como entrada
  digitalWrite(RELE, HIGH);           // Apaga el relé inicialmente

  id = huellaSensor.templateCount;  // Usamos directamente el contador de huellas del sensor
  Serial.println("Cantidad de IDs: ");
  Serial.println(id);

  // Espera que el puerto serial esté listo
  while (!Serial)
    ;

  // Configura la conexión Wi-Fi
  conectarWiFi();
}

void loop() {
  // Verifica si el pulsador fue presionado para registrar una nueva huella
  if (digitalRead(BUTTON_PIN) == LOW) {
    manejarPresionBoton();
  }

  // Verifica si se ha colocado una huella registrada para activar el relé
  int idHuella = obtenerIDHuella();

  if (idHuella != -1) {  // Huella válida
    manejarHuellaValida();
  } else {
    lcd.clear();
    lcd.noBacklight();  // Apaga la retroiluminación del LCD
  }

  delay(50);  // Pequeño retraso para evitar leer repetidamente
}

// Función para inicializar el LCD
void inicializarLCD() {
  lcd.init();       // Inicializa el LCD
  lcd.backlight();  // Enciende la luz de fondo del LCD
  lcd.setCursor(0, 0);
  lcd.print("Bienvenido");
  delay(2000);
}

// Función para inicializar el sensor de huellas
void inicializarSensorHuellas() {
  Serial.begin(9600);
  huellaSensor.begin(57600);
  delay(5);

  if (huellaSensor.verifyPassword()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor encontrado");
    Serial.println("¡Sensor de huellas encontrado!");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor no encontrado");
    Serial.println("No se encontró el sensor de huellas :(");
    while (1) { delay(1); }  // Detener el código si el sensor no se encuentra
  }

  huellaSensor.getParameters();
  huellaSensor.getTemplateCount();
  if (huellaSensor.templateCount == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sin huellas");
    Serial.println("El sensor no contiene datos de huellas. Ejecuta 'enroll'.");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Esperando huella");
    delay(3000);
    Serial.println("Esperando una huella válida...");
  }
  delay(2000);
}

// Función para manejar la presión del botón para registrar una nueva huella
void manejarPresionBoton() {
  Serial.println("Se presionó el pulsador");

  if (tiempoBoton == 0) {
    tiempoBoton = millis();  // Inicia el contador de tiempo de presión
  }

  if (millis() - tiempoBoton >= 5000) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.backlight();
    lcd.print("Modo registro");
    lcd.setCursor(0, 1);
    lcd.print("solicitado");
    delay(3000);  // Espera 3 segundos para mostrar el mensaje en pantalla

    // Mensaje que indica al usuario que coloque su dedo
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Coloca tu dedo");

    unsigned long tiempoEspera = millis();  // Captura el tiempo en que empieza la espera

    // Bucle que espera a que el sensor detecte la huella
    while (true) {
      int idHuella = obtenerIDHuella();  // Intenta obtener el ID de la huella registrada

      if (idHuella != -1) {
        // Si se detecta una huella válida
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Huella");
        lcd.setCursor(0, 1);
        lcd.print("autorizada");
        delay(3000);  // Mostrar mensaje por 2 segundos
        lcd.clear();
        Serial.println("Huella registrada detectada, agregando nueva huella...");
        agregarNuevaHuella();  // Llama a la función para registrar una nueva huella
        break;                 // Sale del bucle porque ya se detectó una huella válida
      }

      // Si no se detecta huella, sigue esperando
      lcd.setCursor(0, 1);
      lcd.print("Esperando huella...");

      // Verifica si han pasado más de 10 segundos sin detectar huella, para salir del bucle
      if (millis() - tiempoEspera >= 10000) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tiempo agotado");
        delay(2000);  // Mostrar mensaje por 2 segundos
        break;        // Sale del bucle después de 10 segundos sin huella
      }
    }

    tiempoBoton = 0;  // Reinicia el tiempo del botón
  }
}

// Función para obtener el ID de la huella registrada
int obtenerIDHuella() {
  uint8_t p = huellaSensor.getImage();  // Captura la imagen de la huella
  if (p != FINGERPRINT_OK) {
    return -1;
  }
  p = huellaSensor.image2Tz();  // Convierte la imagen a una plantilla
  if (p != FINGERPRINT_OK) {
    return -1;
  }
  p = huellaSensor.fingerFastSearch();  // Busca una coincidencia rápida
  if (p != FINGERPRINT_OK) {
    return -1;
  }
  Serial.print("ID de huella: ");
  Serial.println(huellaSensor.fingerID);

  return huellaSensor.fingerID;  // Devuelve el ID de la huella encontrada
}

// Función para manejar la huella válida y activar el relé
void manejarHuellaValida() {
  lcd.setCursor(0, 0);
  lcd.print("Huella valida     ");
  Serial.println("Huella verificada, gracias");
  lcd.backlight();          // Enciende la retroiluminación del LCD
  digitalWrite(RELE, LOW);  // Enciende el relé (comportamiento inverso)
  delay(4000);
  digitalWrite(RELE, HIGH);  // Apaga el relé
  lcd.noBacklight();         // Apaga la retroiluminación del LCD
}

uint8_t agregarNuevaHuella() {
  int p = -1;
  id = id + 1;       // Usamos directamente el contador de huellas del sensor
  int intentos = 0;  // Contador de intentos

  while (intentos < 3) {  // Intentar hasta 3 veces
    Serial.print("Esperando huella válida para registrar como #");
    Serial.println(id);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Apoye un");
    lcd.setCursor(0, 1);
    lcd.print("dedo");

    // Espera hasta que se coloque un dedo para tomar la imagen
    p = FINGERPRINT_NOFINGER;
    while (p != FINGERPRINT_OK) {
      p = huellaSensor.getImage();
      if (p == FINGERPRINT_OK) {
        Serial.println("Imagen tomada");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Imagen lista");

        // Pide al usuario que coloque su dedo para capturar la plantilla
        p = huellaSensor.image2Tz();
        if (p == FINGERPRINT_OK) {
          Serial.println("Imagen convertida");
          lcd.setCursor(0, 1);
          lcd.print("Imagen convertida");
          delay(2000);
        } else {
          Serial.println("Error al convertir imagen");
          lcd.setCursor(0, 1);
          lcd.print("Error al convertir");
          delay(2000);
        }
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Coloca tu dedo");
      }
    }
    Serial.println("Huella agregada");

    // Envia el nuevo ID al servidor (ajustado para tu servidor)
    if (enviarIdAlServidor(id)) {
      lcd.setCursor(0, 0);
      lcd.print("ID guardado en servidor");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Error servidor");
    }
    intentos++;  // Contabiliza un intento
  }
}

// Función para conectar el ESP8266 a la red Wi-Fi
void conectarWiFi() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectando WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi conectado");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);
}

// Función para enviar el ID al servidor mediante HTTP POST
bool enviarIdAlServidor(int id) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String body = "id=" + String(id);
    int httpResponseCode = http.POST(body);  // Realiza la petición POST

    if (httpResponseCode > 0) {
      Serial.print("Respuesta HTTP: ");
      Serial.println(httpResponseCode);
      http.end();
      return true;  // Si se envió correctamente
    } else {
      Serial.print("Error en la solicitud HTTP: ");
      Serial.println(httpResponseCode);
      http.end();
      return false;  // Si ocurrió un error
    }
  }
  return false;
}
