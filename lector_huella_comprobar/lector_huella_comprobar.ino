#include <Adafruit_Fingerprint.h>
#define LED 13

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

bool Switch = true;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  while (!Serial);  // Para Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nIniciando sensor de huellas...");

  // Configurar la velocidad de comunicación con el sensor
  finger.begin(57600);
  delay(5);
  
  if (finger.verifyPassword()) {
    Serial.println("Sensor de huellas encontrado!");
  } else {
    Serial.println("No se encontró el sensor de huellas :(");
    while (1) {
      delay(1);
    }
  }

  finger.getParameters();
  finger.getTemplateCount();
  if (finger.templateCount == 0) {
    Serial.println("El sensor no contiene huellas almacenadas.");
  } else {
    Serial.print("El sensor contiene "); 
    Serial.print(finger.templateCount); 
    Serial.println(" huellas");
  }
}

void loop() {
  Serial.println("Coloque su dedo en el sensor");
  
  // Espera 2 segundos antes de intentar leer la huella
  delay(2000);

  // Verifica si hay un dedo en el lector antes de buscar la huella
  if (finger.getImage() == FINGERPRINT_NOFINGER) {
    // No hay dedo en el lector, no hagas nada
    Serial.println("No se detectó ningún dedo.");
    return;
  }

  // Ahora intenta identificar la huella recorriendo los IDs almacenados
  int value = getFingerprintIDez();

  if (value != -1) {
    Serial.print("Huella reconocida, ID: ");
    Serial.println(value);
    if (Switch) {
      digitalWrite(LED, HIGH);
      Serial.println("Encendiendo LED.");
      Switch = false;
    } else {
      digitalWrite(LED, LOW);
      Serial.println("Apagando LED.");
      Switch = true;
    }
    delay(2000);  // Tiempo para que el usuario vea el resultado
  } else {
    Serial.println("Huella no reconocida.");
    delay(2000);  // Tiempo para permitir al usuario intentar de nuevo
  }
}

// Retorna -1 si falla, de lo contrario retorna el ID de la huella
int getFingerprintIDez() {
  for (int i = 1; i <= finger.templateCount; i++) {
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK) {
      return -1;
    }

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) {
      return -1;
    }

    p = finger.fingerSearch();  // Busca en todos los IDs
    if (p == FINGERPRINT_OK) {
      return finger.fingerID;  // Devuelve el ID si coincide
    }
  }
  return -1;  // Si no se encontró una coincidencia
}
