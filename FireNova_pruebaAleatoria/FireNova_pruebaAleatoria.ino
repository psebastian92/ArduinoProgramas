#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

// ======= CONFIG =======
// const char* WIFI_SSID = "TP-Link_3FE2";
// const char* WIFI_PASS = "26826615";
// const char* HOST = "192.168.0.115";
// const int PORT = 8080;

const char* WIFI_SSID = "Proyectos";
const char* WIFI_PASS = "Fg1289fg";
const char* HOST = "proyectos.fatimarem.edu.ar";
const int PORT = 8081;
// // si tu endpoint es distinto, cámbialo aquí:
// const char* URL = "/FireNova/RecibirDatos";



// si tu endpoint es distinto, cámbialo aquí:
const char* URL = "/WebDinamicaIncendios/RecibirDatos";
// intervalo de envío (ms)
const unsigned long INTERVALO_MS = 5000;

// Contador de generador de datos
int contadorMediciones = 0;

// ======= ESTADO =======
unsigned long t_last = 0;

// ======= HELPERS =======
// float aleatorio en [minV, maxV]
float randFloat(float minV, float maxV) {
  // esp_random() devuelve uint32_t
  const float r01 = (esp_random() / (float)UINT32_MAX);  // [0..1)
  return minV + r01 * (maxV - minV);
}

// genera “mediciones” plausibles
void generarMediciones(String& gas, String& humedadSuelo, String& temperatura, String& humedadAire, String& velocidad) {
  float v_gas = 0.0f;
  float v_hum_suelo = 0.0f;
  float v_temp = 0.0f;
  float v_hum_aire = 0.0f;
  float v_vel_kmh = 0.0f;


  if (contadorMediciones % 5 == 0) {
    v_gas = randFloat(0.0f, 100.0f);       // %
    v_hum_suelo = randFloat(5.0f, 29.0f);  // %
    v_temp = randFloat(30.0f, 45.0f);      // °C
    v_hum_aire = randFloat(10.0f, 95.0f);
    v_vel_kmh = randFloat(30.0f, 60.0f);  // km/h
  } else if (contadorMediciones % 7 == 0) {
    v_gas = randFloat(61.0f, 100.0f);
    v_hum_suelo = randFloat(5.0f, 90.0f);  // %
    v_temp = randFloat(10.0f, 45.0f);      // °C
    v_hum_aire = randFloat(10.0f, 95.0f);  // %
    v_vel_kmh = randFloat(0.0f, 60.0f);    // km/h
  } else {
    v_gas = randFloat(0.0f, 100.0f);       // %
    v_hum_suelo = randFloat(5.0f, 90.0f);  // %
    v_temp = randFloat(10.0f, 45.0f);      // °C
    v_hum_aire = randFloat(10.0f, 95.0f);  // %
    v_vel_kmh = randFloat(0.0f, 60.0f);    // km/h
  }


  // formateamos con 2 decimales y los dejamos como String
  gas = String(v_gas, 2);
  humedadSuelo = String(v_hum_suelo, 2);
  temperatura = String(v_temp, 2);
  humedadAire = String(v_hum_aire, 2);
  velocidad = String(v_vel_kmh, 2);

  contadorMediciones++;
}

// POST simple application/x-www-form-urlencoded
void enviarDatos(const String& gas, const String& humedadSuelo, const String& temperatura, const String& humedadAire, const String& velocidad) {
  WiFiClient client;

  // construimos cuerpo del POST (agregamos proyecto=FireNovA)
  String postData = String("proyecto=") + "FireNovA" + "&gas=" + gas + "&temperatura=" + temperatura + "&humedadAire=" + humedadAire + "&humedadSuelo=" + humedadSuelo + "&velocidad=" + velocidad;

  Serial.printf("➡️  Conectando a %s:%d\n", HOST, PORT);
  if (!client.connect(HOST, PORT)) {
    Serial.println("❌ No se pudo conectar al servidor");
    return;
  }

  // cabeceras HTTP/1.1 + cuerpo
  client.print(String("POST ") + URL + " HTTP/1.1\r\n" + "Host: " + HOST + "\r\n" + "User-Agent: FireNovA-ESP32\r\n" + "Content-Type: application/x-www-form-urlencoded\r\n" + "Content-Length: " + postData.length() + "\r\n" + "Connection: close\r\n\r\n" + postData);

  // leer respuesta
  unsigned long t0 = millis();
  bool ok200 = false;
  while (client.connected() || client.available()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      if (line.startsWith("HTTP/1.1 200")) ok200 = true;
      // opcional: mostrar primeras líneas
      if (millis() - t0 < 1500) Serial.println(line);
    }
  }
  client.stop();

  if (ok200) {
    Serial.println("✅ Datos enviados correctamente (HTTP 200)");
  } else {
    Serial.println("⚠️  Respuesta no-200 o vacía");
  }
}

// ======= SETUP / LOOP =======
void setup() {
  Serial.begin(115200);
  delay(100);

  // semilla para números aleatorios
  esp_random();  // “warm-up”
  uint32_t seed = esp_random();
  randomSeed(seed);

  // WiFi
  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);

  Serial.print("Conectando a WiFi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.println("\n📶 WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  t_last = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - t_last >= INTERVALO_MS) {
    t_last = now;

    // generar strings aleatorios de los mismos campos
    String s_gas, s_humSuelo, s_temp, s_humAire, s_vel;
    generarMediciones(s_gas, s_humSuelo, s_temp, s_humAire, s_vel);

    // log local
    Serial.println("-----------------------------");
    Serial.println("Proyecto: FireNovA");
    Serial.println("Datos simulados:");
    Serial.println("  gas            = " + s_gas + " %");
    Serial.println("  temperatura    = " + s_temp + " °C");
    Serial.println("  humedad_aire   = " + s_humAire + " %");
    Serial.println("  humedad_suelo  = " + s_humSuelo + " %");
    Serial.println("  velocidad      = " + s_vel + " km/h");

    // enviar
    enviarDatos(s_gas, s_humSuelo, s_temp, s_humAire, s_vel);
  }

  // mantener WiFi vivo (por si pierde conexión)
  wifiMulti.run();
}
