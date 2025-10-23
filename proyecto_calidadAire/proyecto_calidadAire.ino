#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Pines para la comunicación serial con el módulo GPS
const int RX_PIN = 10;  // Conectar TX del módulo GPS a 10
const int TX_PIN = 11;  // Conectar RX del módulo GPS a 11

// Inicializa el objeto SoftwareSerial para la comunicación con el módulo GPS
SoftwareSerial gpsSerial(RX_PIN, TX_PIN);

// Inicializa el objeto TinyGPS++
TinyGPSPlus gps;

const int pinSensorPolvo = A0;
const int pinLed = 7;
const int adcMaximo = 1023;
const float voltajeVcc = 5.0;

// Configuración de MQ2
const float valorMQ2_1ppm = 0.1; // Valor de calibración para el MQ2 (ajustar según datasheet y pruebas)
const float alertaPeligro = 500; // Umbral para alerta de peligro (ajustar según necesidades)

float latitud;
float longitud;
float latitudManual;
float longitudManual;
float nuevaLatitud = 0;
float nuevaLongitud = 0;
// Variables para la gestión de tiempo
unsigned long previousMillis = 0;  // Almacena el tiempo anterior para millis()
const long interval = 60000;       // Intervalo para aumentar el tiempo (1 minuto)

int year = 2024;
int month = 8;
int day = 18;
int hour = 10;
int minute = 0;
int second = 0;

// Días de la semana en español
String dayAsString(const int day) {
  switch (day) {
    case 0: return "Sábado";
    case 1: return "Domingo";
    case 2: return "Lunes";
    case 3: return "Martes";
    case 4: return "Miércoles";
    case 5: return "Jueves";
    case 6: return "Viernes";
    default: return "(día desconocido)";
  }
}

// Obtén el día de la semana basado en la fecha actual
int calculateDayOfWeek(int d, int m, int y) {
  if (m < 3) {
    m += 12;
    y -= 1;
  }
  int K = y % 100;
  int J = y / 100;
  int h = (d + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 - 2 * J) % 7;
  return (h + 7) % 7;  // Ajusta el resultado para que esté en el rango 0-6
}

float leerDensidad() {
  digitalWrite(pinLed, LOW);
  delayMicroseconds(280);
  int valorAdc = analogRead(pinSensorPolvo);
  float voltajeSensor = voltajeVcc * valorAdc / adcMaximo;
  float densidad = 0.170 * voltajeSensor - 0.1;  // mg/m^3
  digitalWrite(pinLed, HIGH);

  if (densidad >= 0) {
    return densidad * 1000;  // ug/m^3
  }
  else {
    return densidad * -1000;
  }

}

float leerMQ2PPM() {
  int valorAdc = analogRead(1);
  return valorAdc;
}

void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  delay(1000);

  gpsSerial.begin(9600); // Inicialización de la comunicación serial para el módulo GPS

  // Ingresar manualmente la latitud
  Serial.println("Ingresa la latitud: ");
  while (Serial.available() == 0) {} // Espera la entrada del usuario
  String latitudStr = Serial.readStringUntil('\n');
  latitudManual = latitudStr.toFloat();
  Serial.print("Latitud ingresada: ");
  Serial.println(latitudManual, 6);

  // Ingresar manualmente la longitud
  Serial.println("Ingresa la longitud: ");
  while (Serial.available() == 0) {} // Espera la entrada del usuario
  String longitudStr = Serial.readStringUntil('\n');
  longitudManual = longitudStr.toFloat();
  Serial.print("Longitud ingresada: ");
  Serial.println(longitudManual, 6);

  // Ingresar manualmente la fecha
  Serial.println("Ingresa el año: ");
  while (Serial.available() == 0) {}
  String yearStr = Serial.readStringUntil('\n');
  year = yearStr.toInt();

  Serial.println("Ingresa el mes (1-12): ");
  while (Serial.available() == 0) {}
  String monthStr = Serial.readStringUntil('\n');
  month = monthStr.toInt();

  Serial.println("Ingresa el día (1-31): ");
  while (Serial.available() == 0) {}
  String dayStr = Serial.readStringUntil('\n');
  day = dayStr.toInt();

  Serial.println("Ingresa la hora (0-23): ");
  while (Serial.available() == 0) {}
  String hourStr = Serial.readStringUntil('\n');
  hour = hourStr.toInt();

  Serial.println("Ingresa el minuto (0-59): ");
  while (Serial.available() == 0) {}
  String minuteStr = Serial.readStringUntil('\n');
  minute = minuteStr.toInt();

  Serial.println("Configuración completa.");
}

void loop() {
  bool newData = false;
  unsigned long currentMillis = millis();  // Obtiene el tiempo actual en milisegundos

  // Lee los datos del módulo GPS
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      newData = true;
    }
  }

  // Si se recibieron nuevos datos del GPS
  if (newData) { // Muestra las coordenadas GPS actuales
    latitud = gps.location.lat();
    longitud = gps.location.lng();

    if (nuevaLatitud != latitud && nuevaLongitud != longitud) {
      latitudManual == latitud;
      longitudManual == longitud;
      nuevaLatitud = latitud;
      nuevaLongitud = longitud;
      Serial.println("Señal GPS recibida.");
      Serial.print("Nuevas coordenadas GPS: ");
      Serial.print(latitud, 6);
      Serial.print(", ");
      Serial.println(longitud, 6);

    } else {
      Serial.print("Coordenadas GPS: ");
      Serial.print(latitudManual, 6);
      Serial.print(", ");
      Serial.println(longitudManual, 6);
    }

  } else  {
    Serial.print("Coordenadas GPS: ");
    Serial.print(latitudManual, 6);
    Serial.print(", ");
    Serial.println(longitudManual, 6);
  }

  // Incrementa la hora y los minutos cada minuto
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Incrementa minutos
    minute += 1;
    if (minute >= 60) {
      minute = 0;
      hour += 1;
      if (hour >= 24) {
        hour = 0;
        day += 1;
        // Ajusta el mes y el año si el día supera el número de días del mes
        if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 31) {
          day = 1;
          month += 1;
        } else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
          day = 1;
          month += 1;
        } else if (month == 2 && day > 28) {  // Febrero, sin considerar años bisiestos
          day = 1;
          month += 1;
        }
        // Incrementa el año si el mes supera 12
        if (month > 12) {
          month = 1;
          year += 1;
        }
      }
    }
  }

  // Obtén el día de la semana
  int dayOfWeek = calculateDayOfWeek(day, month, year);

  // Muestra la fecha y hora
  Serial.print(dayAsString(dayOfWeek));
  Serial.print(" ");
  Serial.print(day);
  Serial.print('/');
  Serial.print(month);
  Serial.print('/');
  Serial.print(year);
  Serial.print(" Hora: ");
  Serial.print(hour);
  Serial.print(':');
  Serial.println(minute);

  // Obtén la densidad de polvo
  Serial.print("Partículas disueltas en aire: ");
  Serial.print(leerDensidad());
  Serial.println(" ug/m^3");

  // Obtén el valor de PPM del MQ2
  float ppm = leerMQ2PPM();
  Serial.print("Concentración de gas MQ2: ");
  Serial.print(ppm);
  Serial.println(" PPM");

  // Verifica si hay una alerta de peligro
  if (ppm > alertaPeligro) {
    Serial.println("¡Alerta de peligro! Concentración de gases contaminantes alta.");
  } else {
    Serial.println("Valores estables de gases contaminantes.");
  }

  delay(3000); // Pausa de 3 segundos
  Serial.println("----------");
}
