#include <WiFi.h>
#include "DHT.h"
#include <DHT_U.h>
#define DHTPIN 4
#define DHTTYPE DHT11


DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "TP-Link_3FE2";
const char* password = "26826615";
const char* host = "prometecserver.ddns.net";

void setup() {

   Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht.begin();
  pinMode (4, INPUT);
Serial.begin(9600);
Serial.println();
Serial.print("Conectando a Wifi: ");
Serial.println(ssid);

WiFi.begin (ssid, password);

while (WiFi.status() != WL_CONNECTED) {
  delay (500);
  Serial.print(".");
}

Serial.println("");
Serial.println("WiFi conectado! ");
Serial.println("Dirección IP: ");
Serial.println(WiFi.localIP());

}

void loop() {
  int err;
  float temp, hum;
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  
  // revisa si retorna un valor valido, de lo contrario hay un error
  if (isnan(temp) || isnan(hum)) 
    {
      Serial.println("Failed to read from DHT");
    }
  else {

Serial.println("Conectando al servidor: ");
Serial.println(host);

 WiFiClient client;
 const int httpPort = 80;
 if (!client.connect (host, httpPort)){
  Serial.println ("Conexión fallida :(");
  return;  
 }

 

String url = "/dhtll.php";
String key = "?pass=1234";
String dato1 = "?temperatura = ";
String dato2 = "?humedad = ";

Serial.print("Solicitando URL: ");
Serial.println(url);

client.print (String ("Obteniendo: ") + url + key + dato1 + temp + dato2 + hum +"HTTP/l.l\r\n" + "HOST: " + host + "\r\n" + "Conexión: cerrar \r\n\r\n");

unsigned long timeout = millis ();

while (client.available () == 0) {
  if (millis () - timeout > 5000){
      Serial.println(">>> Se acabo el tiempo de espera");
      client.stop ();
      return;
    
  }
  
}

while (client.available () ) {
  String line = client.readStringUntil ('\r');
  Serial.print (line);
}



Serial.println();
Serial.println("Cerrando conexión");

delay (60000);

}
}
