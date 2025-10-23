#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// --- Pines ---
#define SDA_PIN   21      // I2C SDA
#define SCL_PIN   22      // I2C SCL
#define LCD_ADDR  0x27    // tu backpack

#define DHTPIN    4       // pin del DHT11
#define DHTTYPE   DHT11
#define MQ2_PIN   34      // ADC1 (solo entrada, ideal)

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

// --- ADC ---
const float VREF   = 3.3f;   // referencia efectiva (asume 3.3 V)
const int   ADCMAX = 4095;   // 12 bits

int readMQ2(int n = 16) {
  long acc = 0;
  for (int i = 0; i < n; i++) {
    acc += analogRead(MQ2_PIN);
    delay(5);
  }
  return acc / n; // promedio para reducir ruido
}

void setup() {
  Serial.begin(115200);

  // MQ-2 en ADC a escala completa (~3.3 V)
  analogSetPinAttenuation(MQ2_PIN, ADC_11db); // full-scale ~3.3 V

  // I2C + LCD (opcional)
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("MQ2 + DHT11");
  lcd.setCursor(0,1); lcd.print("Iniciando...");

  // DHT11
  dht.begin();

  delay(1500);
}

void loop() {
  // --- DHT11 (max ~1 Hz) ---
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  // --- MQ-2 ---
  int raw = readMQ2();
  float vmq = (raw * VREF) / ADCMAX;   // voltaje en GPIO34 tras el divisor

  // ---- Serial ----
  Serial.print("T="); Serial.print(temp); Serial.print(" C  ");
  Serial.print("H="); Serial.print(hum);  Serial.print(" %  ");
  Serial.print("MQ2raw="); Serial.print(raw);
  Serial.print("  V="); Serial.println(vmq, 2);

  // ---- LCD ----
  lcd.clear();
  lcd.setCursor(0,0);
  if (isnan(temp) || isnan(hum)) {
    lcd.print("DHT11 error   ");
  } else {
    char l1[17];
    snprintf(l1, sizeof(l1), "T:%4.1fC H:%2.0f%%", temp, hum);
    lcd.print(l1);
  }
  lcd.setCursor(0,1);
  char l2[17];
  snprintf(l2, sizeof(l2), "MQ2:%4d %4.2fV", raw, vmq);
  lcd.print(l2);

  delay(2000); // DHT11: no más de ~1 lectura/segundo
}
