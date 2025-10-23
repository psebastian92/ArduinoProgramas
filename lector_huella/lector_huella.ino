#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(2, 3); // TX/RX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

void setup() {
  Serial.begin(9600);
  Serial.println("Coloca el dedo");
  
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nRegistro de huellas dactilares");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Sensor de huellas dactilares encontrado!");
  } else {
    Serial.println("No se encontró el sensor de huellas dactilares :(");
    while (1) {
      delay(1);
    }
  }

  finger.getParameters();
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (!Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop() { // run over and over again
  Serial.println("Listo para registrar una huella dactilar!");
  Serial.println("Por favor, ingresa el ID # (de 1 a 127) en el que deseas guardar esta huella...");
  id = readnumber();
  if (id == 0) { // ID #0 no permitido, intenta de nuevo!
    return;
  }
  Serial.print("Registrando ID #");
  Serial.println(id);

  while (!getFingerprintEnroll());
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Esperando huella válida para registrar como #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Imagen tomada");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Error de comunicación");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Error al capturar la imagen");
        break;
      default:
        Serial.println("Error desconocido");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagen convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagen demasiado desordenada");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de comunicación");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("No se encontraron características en la huella");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("No se encontraron características en la imagen");
      return p;
    default:
      Serial.println("Error desconocido");
      return p;
  }

  Serial.println("Retira el dedo");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Coloca el mismo dedo nuevamente");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Imagen tomada");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Error de comunicación");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Error al capturar la imagen");
        break;
      default:
        Serial.println("Error desconocido");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagen convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagen demasiado desordenada");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de comunicación");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("No se encontraron características en la huella");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("No se encontraron características en la imagen");
      return p;
    default:
      Serial.println("Error desconocido");
      return p;
  }

  // OK converted!
  Serial.print("Creando modelo para #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("¡Huellas coinciden!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Error de comunicación");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Las huellas no coinciden");
    return p;
  } else {
    Serial.println("Error desconocido");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("¡Guardado!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Error de comunicación");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("No se pudo guardar en esa ubicación");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error al escribir en la memoria flash");
    return p;
  } else {
    Serial.println("Error desconocido");
    return p;
  }

  return true;
}
