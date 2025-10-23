#include <IRremote.h>  // Librería IRremote moderna

#define receptorIR 12   // Pin del receptor IR

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Inicializa el receptor IR
  IrReceiver.begin(receptorIR, ENABLE_LED_FEEDBACK);
  Serial.println("Receptor IR listo para recibir señales...");
}

void loop() {
  // Si se recibió una señal
  if (IrReceiver.decode()) {
    Serial.print("Código recibido: 0x");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

    // Limpia para recibir el siguiente
    IrReceiver.resume();
  }
}
