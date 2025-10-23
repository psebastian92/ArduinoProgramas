#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial); // Espera a que el puerto serie esté listo
  Serial.println("\nEscaneando bus I2C...");
}

void loop() {
  byte error, direccion;
  int dispositivos = 0;

  for (direccion = 1; direccion < 127; direccion++) {
    Wire.beginTransmission(direccion);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo I2C encontrado en 0x");
      if (direccion < 16) Serial.print("0");
      Serial.println(direccion, HEX);

      dispositivos++;
    }
  }

  if (dispositivos == 0) {
    Serial.println("No se encontraron dispositivos I2C\n");
  } else {
    Serial.println("Escaneo completado\n");
  }

  delay(5000); // Espera 5 segundos antes de volver a escanear
}
