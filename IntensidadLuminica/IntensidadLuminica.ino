#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <BH1750.h>

WiFiMulti wifiMulti;

// 🔹 Objeto dinámico (solo se crea si el sensor existe)
BH1750* lightMeter1 = nullptr;

// Pines LEDs Aula 1
const int led1 = 23;
const int led2 = 21;
const int led3 = 17;
const int led4 = 16;

// Pin LED integrado (o led físico) para comprobar conexión WiFi
const int ledTestigoWifi = 2;

// Pines I2C BH1750
const int SDA_PIN = 18;
const int SCL_PIN = 19;

// URLs
const char* serverLuzUrl = "http://proyectos.fatimarem.edu.ar:8081/IntensidadLuminica/RecibirDatosESP32";
const char* serverControlUrl = "http://proyectos.fatimarem.edu.ar:8081/IntensidadLuminica/vistas/control_leds.jsp";

// Intervalo de envío
const unsigned long intervaloEnvio = 5000;
unsigned long ultimoEnvio = 0;

bool sensorOk = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  // 🔹 Escaneo rápido para detectar BH1750
  Wire.beginTransmission(0x23);
  if (Wire.endTransmission() == 0) {
    lightMeter1 = new BH1750(0x23);
    if (lightMeter1->begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
      Serial.println("✔️ Sensor Aula 1 iniciado correctamente.");
      sensorOk = true;
    }
  } else {
    Serial.println("⚠️ No se detectó BH1750. Continuando sin sensor...");
    sensorOk = false;
  }

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(ledTestigoWifi, OUTPUT);
  apagarLeds();

  int intentosConexion = 0;
  wifiMulti.addAP("Proyectos", "Fg1289fg");
  wifiMulti.addAP("TP-Link_3FE2", "26826615");  // Red 2
  Serial.print("Conectando a WiFi");
  while (wifiMulti.run() != WL_CONNECTED && intentosConexion <= 10) {
    delay(1000);
    Serial.print(".");
    intentosConexion++;
  }

  if (intentosConexion >= 10) {
    Serial.println("\nNo se pudo conectar a WiFi");
  } else {
    Serial.println("\n✅ Conectado a WiFi");
    Serial.print("IP local: ");
    Serial.println(WiFi.localIP());
    digitalWrite(ledTestigoWifi, HIGH);
  }
}

void loop() {
  float lux1 = -1;

  // --- Lectura del sensor solo si existe ---
  if (sensorOk && lightMeter1 != nullptr) {
    float lectura = lightMeter1->readLightLevel();
    if (!isnan(lectura) && lectura >= 0) {
      lux1 = lectura;
      Serial.printf("🌞 Aula 1: %.2f lx\n", lux1);
    } else {
      Serial.println("⚠️ Lectura inválida del sensor.");
    }
  } else {
    Serial.println("⚠️ Sensor no disponible. Usando solo modo manual.");
  }

  // --- Pedir modo al servidor ---
  if (wifiMulti.run() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverControlUrl + String("?aula=aula1"));
    int codigo = http.GET();

    if (codigo == 200) {
      String respuesta = http.getString();
      Serial.println("📩 Respuesta del servidor:");
      Serial.println(respuesta);

      bool modoAuto = respuesta.indexOf("modoAuto=true") >= 0;

      if (modoAuto) {
        if (sensorOk && lux1 >= 0) {
          controlarLeds(lux1);  // automático válido
        } else {
          Serial.println("⚠️ Se pidió automático, pero no hay sensor → sigo en manual.");
          digitalWrite(led1, respuesta.indexOf("led1=ON") >= 0 ? LOW : HIGH);
          digitalWrite(led2, respuesta.indexOf("led2=ON") >= 0 ? LOW : HIGH);
          digitalWrite(led3, respuesta.indexOf("led3=ON") >= 0 ? LOW : HIGH);
          digitalWrite(led4, respuesta.indexOf("led4=ON") >= 0 ? LOW : HIGH);
        }
      } else {
        // Modo manual puro
        digitalWrite(led1, respuesta.indexOf("led1=ON") >= 0 ? LOW : HIGH);
        digitalWrite(led2, respuesta.indexOf("led2=ON") >= 0 ? LOW : HIGH);
        digitalWrite(led3, respuesta.indexOf("led3=ON") >= 0 ? LOW : HIGH);
        digitalWrite(led4, respuesta.indexOf("led4=ON") >= 0 ? LOW : HIGH);
      }
    }
    http.end();
  }

  // --- Enviar datos solo si hay sensor válido ---
  if (millis() - ultimoEnvio >= intervaloEnvio && sensorOk && lux1 >= 0) {
    ultimoEnvio = millis();
    if (wifiMulti.run() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverLuzUrl);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String datos = "aula=aula1&valor=" + String(lux1);
      int codigo = http.POST(datos);

      if (codigo > 0) {
        Serial.print("✔️ Aula1 enviado: ");
        Serial.println(datos);
      } else {
        Serial.print("❌ Error Aula1 al enviar: ");
        Serial.println(codigo);
      }
      http.end();
    }
  }

  delay(500);
}

// ----------------------
// Automático
void controlarLeds(float lux) {
  if (lux < 50) encenderLeds(4);
  else if (lux < 150) encenderLeds(3);
  else if (lux < 300) encenderLeds(2);
  else if (lux < 500) encenderLeds(1);
  else encenderLeds(0);
}

void encenderLeds(int cantidad) {
  digitalWrite(led1, cantidad >= 1 ? LOW : HIGH);
  digitalWrite(led2, cantidad >= 2 ? LOW : HIGH);
  digitalWrite(led3, cantidad >= 3 ? LOW : HIGH);
  digitalWrite(led4, cantidad >= 4 ? LOW : HIGH);
}

void apagarLeds() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  digitalWrite(ledTestigoWifi, LOW);
}
