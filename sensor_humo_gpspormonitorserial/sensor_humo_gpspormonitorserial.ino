const int pinSensor = A0;
const int pinLed = 7;
const int adcMaximo = 1023;
const float voltajeVcc = 5.0;

float latitud;
float longitud;

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
  int valorAdc = analogRead(pinSensor);
  float voltajeSensor = voltajeVcc * valorAdc / adcMaximo;
  float densidad = 0.170 * voltajeSensor - 0.1;  // mg/m^3
  digitalWrite(pinLed, HIGH);
  return densidad * 1000;  // ug/m^3
}

void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  delay(1000);

  // Ingresar manualmente la latitud
  Serial.println("Ingresa la latitud: ");
  while (Serial.available() == 0) {} // Espera la entrada del usuario
  String latitudStr = Serial.readStringUntil('\n');
  latitud = latitudStr.toFloat();
  Serial.print("Latitud ingresada: ");
  Serial.println(latitud, 6);

  // Ingresar manualmente la longitud
  Serial.println("Ingresa la longitud: ");
  while (Serial.available() == 0) {} // Espera la entrada del usuario
  String longitudStr = Serial.readStringUntil('\n');
  longitud = longitudStr.toFloat();
  Serial.print("Longitud ingresada: ");
  Serial.println(longitud, 6);

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
  unsigned long currentMillis = millis();  // Obtiene el tiempo actual en milisegundos

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

  // Muestra las coordenadas GPS ingresadas
  Serial.print("Coordenadas GPS: ");
  Serial.print(latitud, 6);
  Serial.print(", ");
  Serial.println(longitud, 6);

  delay(3000); // Pausa de 3 segundos
  Serial.println("----------");
}
