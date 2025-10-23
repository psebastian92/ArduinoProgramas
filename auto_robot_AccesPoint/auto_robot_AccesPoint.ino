#include <WiFi.h>

const char* ssid = "TestAP";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  bool apStarted = WiFi.softAP(ssid, password);
  delay(1000);
  if (apStarted) {
    Serial.println("AP started");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("AP failed");
  }
}

void loop() {}
