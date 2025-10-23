#include <WiFiMulti.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

WiFiMulti wifiMulti;

// ------- DEFINICIÓN DE PINES -------
#define SENSOR_HUMEDAD_PIN 34  // GPIO34: humedad del suelo
#define MQ_PIN 35              // GPIO35: sensor MQ gas
#define DHTPIN 32              // GPIO32: sensor DHT11
#define DHTTYPE DHT11

#define SPEED_SENSOR_PIN 27  // GPIO27: LM393 Speed Sensor

DHT dht(DHTPIN, DHTTYPE);

// ------- VARIABLES PARA SENSOR DE VELOCIDAD -------
volatile unsigned int contadorPulsos = 0;
unsigned long tiempoAnterior = 0;
const int PULSOS_POR_VUELTA = 20;  // o 40 si usás CHANGE
const float K_MS_POR_HZ = 10.33f;  // ≈ m/s por Hz (aproximación práctica)

void IRAM_ATTR contarPulsos() {
  contadorPulsos++;
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Configuración del sensor de velocidad
  pinMode(SPEED_SENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SPEED_SENSOR_PIN), contarPulsos, RISING);

  // Conexión WiFi
 wifiMulti.addAP("Proyectos", "Fg1289fg");
  Serial.print("Conectando a WiFi");
  int intentosWifi = 0;
  while (wifiMulti.run() != WL_CONNECTED && intentosWifi <= 5) {
    Serial.print(".");
    delay(500);
    intentosWifi++;
  }

  if (intentosWifi <= 5) {
    Serial.println("\nConectado a WiFi");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nNo se pudo establecer la conexión");
  }


  delay(2000);
}

void loop() {
  float gas = medirGases();
  float humedadSuelo = medirHumedadSuelo();
  float temperatura, humedadAire;
  medirTempHum(temperatura, humedadAire);
  float velocidad = medirVelocidad();  // km/h

  enviarDatos(gas, humedadSuelo, temperatura, humedadAire, velocidad);

  Serial.println("-----------------------------");
  delay(2000);
}

// ===== FUNCIONES =====

// Humedad del suelo
float medirHumedadSuelo() {
  int valor = analogRead(SENSOR_HUMEDAD_PIN);
  float humedad = map(valor, 0, 4095, 100, 0);
  Serial.print("Humedad del suelo: ");
  Serial.print(humedad);
  Serial.println("%");
  return humedad;
}

// Gas MQ
float medirGases() {
  int valor = analogRead(MQ_PIN);
  float porcentaje = (valor / 4095.0) * 100.0;
  Serial.print("Nivel de gas: ");
  Serial.print(porcentaje);
  Serial.println("%");
  return porcentaje;
}

// DHT11
void medirTempHum(float &temperatura, float &humedad) {
  temperatura = dht.readTemperature();
  humedad = dht.readHumidity();

  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Error al leer el DHT11. Reintentando...");
    delay(1000);
    temperatura = dht.readTemperature();
    humedad = dht.readHumidity();
  }

  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Error persistente en DHT11.");
    temperatura = humedad = 0;
  }

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  Serial.print("Humedad aire: ");
  Serial.print(humedad);
  Serial.println(" %");
}

// Velocidad LM393 (km/h)
float medirVelocidad() {
  static unsigned long t0 = millis();
  static float v_kmh_last = 0.0f;

  unsigned long t1 = millis();
  unsigned long dt_ms = t1 - t0;
  if (dt_ms < 500) {  // actualizá ~2 veces por segundo
    return v_kmh_last;
  }

  // leer y limpiar contador de forma atómica
  noInterrupts();
  unsigned int p = contadorPulsos;
  contadorPulsos = 0;
  interrupts();

  float dt_s = dt_ms / 1000.0f;
  float pulsos_por_seg = p / dt_s;
  float f_hz = pulsos_por_seg / PULSOS_POR_VUELTA;

  // v ≈ 0.85 m/s por Hz  → km/h
  float v_ms = K_MS_POR_HZ * f_hz;
  float v_kmh = v_ms * 3.6f;

  t0 = t1;
  v_kmh_last = v_kmh;

  // (opcional) log compacto
  Serial.print("Viento ≈ ");
  Serial.print(v_kmh, 1);
  Serial.println(" km/h");

  return v_kmh;
}

// Envío de datos por HTTP
void enviarDatos(float gas, float humedadSuelo, float temperatura, float humedadAire, float velocidad) {
  WiFiClient client;
  const char *host = "proyectos.fatimarem.edu.ar";
  const int port = 8081;
  String url = "/FireNova/RecibirDatos";
  // const char* host = "proyectos.fatimarem.edu.ar";
  // const int port = 8081;
  // String url = "/FireNova/RecibirDatos";

  // Crear el cuerpo del POST
  String postData = "gas=" + String(gas, 2) + "&temperatura=" + String(temperatura, 2) + "&humedadAire=" + String(humedadAire, 2) + "&humedadSuelo=" + String(humedadSuelo, 2) + "&velocidad=" + String(velocidad, 2);

  if (client.connect(host, port)) {
    // Encabezados HTTP para POST
    client.print(String("POST ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Content-Type: application/x-www-form-urlencoded\r\n" + "Content-Length: " + postData.length() + "\r\n" + "Connection: close\r\n\r\n");

    // Enviar datos en el cuerpo
    client.print(postData);

    // Leer la respuesta del servidor
    while (client.connected() || client.available()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');

        if (line.startsWith("HTTP/1.1 200")) {
          Serial.println("✅ Datos recibidos correctamente (HTTP 200 OK)");
        }
      }
    }
  } else {
    Serial.println("❌ Error al conectar con el servidor");
  }
}
