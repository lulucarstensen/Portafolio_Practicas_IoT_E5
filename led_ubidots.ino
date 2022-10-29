#include <WiFi.h>
#include <DHT.h>
/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"
#define pin1 14
#define LED 27
#define DEVICE_LABEL "testmqtt" // Nombre del dispositivo a crear

DHT dht1(pin1, DHT11); //El sensor de temp y humedad azul.

/****************************************
 * Define Instances and Constants
 ****************************************/
const char* UBIDOTS_TOKEN = "BBFF-xrd8Nlr39sRwt22WX2qRHbV5OK2AQF";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "Carstensen";      // Put here your Wi-Fi SSID
const char* WIFI_PASSWORD = "FAMcars2122";      // Put here your Wi-Fi password


char topic[700];
char payload[300]; // Leer y entender el payload aqui una de tantas referencias "https://techterms.com/definition/payload"
int PORT=80;
int LEDON=0;
char const * VARIABLE_LABEL_1 = "ledlighton";
char const * VARIABLE_LABEL_2 = "temperatura";

WiFiClient client;
/****************************************
 * Auxiliar Functions
 ****************************************/

void SendToUbidots(char* payload) {

  /* Builds the request POST - Please reference this link to know all the request's structures https://ubidots.com/docs/api/ */
  sprintf(topic, "POST /api/v1.6/devices/%s/?force=true HTTP/1.1\r\n", DEVICE_LABEL);
  sprintf(topic, "%sHost: industrial.api.ubidots.com\r\n", topic);
  sprintf(topic, "%sX-Auth-Token: %s\r\n", topic, UBIDOTS_TOKEN);
  sprintf(topic, "%sContent-Type: application/json\r\n", topic);
  sprintf(topic, "%sContent-Length: %d\r\n\r\n", topic, 36);
  sprintf(topic, "%s%s\r\n", topic, payload);
  
  /* Connecting the client */
  client.connect("industrial.api.ubidots.com", PORT); 

  if (client.connected()) {
    /* Sends the request to the client */
    Serial.println(topic);
    client.print(topic);
  } else {
    Serial.println("Connection Failed ubidots - Try Again"); 
  }  
    
  /* Reads the response from the server */
  while (client.available()) {
    char c = client.read();
    //Serial.print(c); // Uncomment this line to visualize the response on the Serial Monitor
  }

  /* Disconnecting the client */
  client.stop();
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID); 
  /* Connects to AP */
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.println("WiFi connected");

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  dht1.begin();
}

void loop() {
  float t1 = dht1.readTemperature();
  Serial.print("Temperatura DHT11: ");
  Serial.print(t1);
  Serial.println(" ºC.");
  
  if(t1>28){
    Serial.print("Encendido");
    LEDON=1;
    digitalWrite(LED, HIGH);
  }
  else if(t1<25){
    Serial.print("Apagado");
    LEDON=0;
    digitalWrite(LED, LOW);
  }

  delay(100);
  
  sprintf(payload,"{\"");
  sprintf(payload, "%s%s\":%d", payload, VARIABLE_LABEL_1, LEDON);
  sprintf(payload, "%s,\"%s\":%.0f", payload, VARIABLE_LABEL_2, t1);
  sprintf(payload, "%s}", payload);

  delay(10);
  SendToUbidots(payload);
  /* Prints the data posted on the Serial Monitor */
  Serial.println("Posting data to Ubidots");

  delay(15000); // 15 segundos en milisegundos entre publicaciones en ubidots
}
