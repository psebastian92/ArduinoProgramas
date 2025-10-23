#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>  // Incluye la librería para HTTP

const char* ssid = "FliaMunoz";     // SSID de tu red Wi-Fi
const char* password = "26826615";  // Contraseña de tu red Wi-Fi

const char* server = "http://192.168.0.14:8080/esp8266BD/DatosRecibidos";  // URL completa del servlet

WiFiClient client;   // Mantenemos el cliente WiFi fuera del loop
HTTPClient http;     // Mantenemos el objeto HTTP fuera del loop para evitar crearlo y destruirlo repetidamente

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);

  // Espera hasta que el módulo se conecte a la red Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a la red WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {                         // Verifica que estés conectado a WiFi
    int nivelSonido = analogRead(A0);                          // Lee el valor del KY-037 (conectado al pin A0)
    double nivelSonidoDB = map(nivelSonido, 0, 1023, 0, 100);  // Mapea el valor a dB (ajusta según tus necesidades)
    
    // Imprime los valores leídos para verificar
    Serial.print("Nivel de sonido (A0): ");
    Serial.println(nivelSonido);
    Serial.print("Nivel de sonido en dB: ");
    Serial.println(nivelSonidoDB);
    
    // Especifica la URL completa del servidor
    http.begin(client, server);  // Usa el objeto client y la URL
    
    // Especifica el tipo de contenido
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepara los datos a enviar
    String postData = "nivelsonido=" + String(nivelSonidoDB);

    // Envía la solicitud POST y obtiene la respuesta
    int httpResponseCode = http.POST(postData);
    
    if (httpResponseCode == 200) {
      // Si la solicitud fue exitosa y el código es 200
      Serial.println("Status 200: Respuesta correcta");
      
    } else {
      // Si hubo un error o el código de respuesta no es 200
      Serial.println("Error o código diferente de 200: " + String(httpResponseCode));
    }

    // Termina la conexión con el servidor
    http.end();
  } else {
    Serial.println("Error: No conectado a WiFi");
    
    // Intentar reconectar si la conexión se pierde
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    Serial.println("Reconectando a WiFi...");
  }

  // Espera un tiempo antes de la próxima lectura
  delay(5000);  // Espera 5 segundos
}
