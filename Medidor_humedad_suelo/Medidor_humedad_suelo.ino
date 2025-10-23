#include <ESP8266WiFi.h>


// ----------------------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------------------
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "TP-Link_3FE2";
char pass[] = "26826615";
String GAS_ID = "AKfycbwKOWsBnSnFN77iocU4xvFJ8YvHwybOsL-MiZqEcU3CZ_sSUcdFDdwA0g7sCNQ6VEj8"; //--> spreadsheet script ID

//Your Domain name with URL path or IP address with path
const char* host = "script.google.com"; // only google.com not https://google.com

// ----------------------------------------------------------------------------------------------

#define UPDATE_INTERVAL_HOUR  (0)
#define UPDATE_INTERVAL_MIN   (0)
#define UPDATE_INTERVAL_SEC   (30)

#define UPDATE_INTERVAL_MS    ( ((UPDATE_INTERVAL_HOUR*60*60) + (UPDATE_INTERVAL_MIN * 60) + UPDATE_INTERVAL_SEC ) * 1000 )

// ----------------------------------------------------------------------------------------------

int humedad;
String estadoTierra;

// ----------------------------------------------------------------------------------------------
void update_google_sheet()
{
    Serial.print("connecting to ");
    Serial.println(host);
  
    // Use WiFiClient class to create TCP connections
    WiFiClientSecure client;
    const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!
    
    client.setInsecure(); // this is the magical line that makes everything work
    
    if (!client.connect(host, httpPort)) { //works!
      Serial.println("connection failed");
      return;
    }
       
    //----------------------------------------Processing data and sending data
    String url = "/macros/s/" + GAS_ID + "/exec?moisture=";

    url += String(estadoTierra);
    
    Serial.print("Requesting URL: ");
    Serial.println(url);
  
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
  
    Serial.println();
    Serial.println("closing connection");  
}

// ----------------------------------------------------------------------------------------------
void setup()
{
  // Debug console
  Serial.begin(9600);

  // Digital output pin


//----------------------------------------Wait for connection
  Serial.print("Connecting");
  WiFi.begin(ssid, pass); //--> Connect to your WiFi router
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
  }

}

// ----------------------------------------------------------------------------------------------
unsigned long time_ms;
unsigned long time_1000_ms_buf;
unsigned long time_sheet_update_buf;
unsigned long time_dif;

void loop()
{

  // Read and print serial data every 1 sec
  if ( time_dif >= 1000 ) // 1sec 
  {
    time_1000_ms_buf = time_ms;
   humedad = analogRead(0);

   if (humedad > 700) {
      estadoTierra = "Regar!!!";
   }
   else {
    estadoTierra = "TierraOK";
   }
  
    // Print serial messages
    Serial.print("Humedad: " + String(humedad) + " %");
    Serial.print("\t");
    delay(500);
  }

  // Update data to google sheet in specific period
  time_ms = millis();
  time_dif = time_ms - time_sheet_update_buf;  
  if ( time_dif >= UPDATE_INTERVAL_MS ) // Specific period
  {
    time_sheet_update_buf = time_ms;
    update_google_sheet();
  }
 
}
