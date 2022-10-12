#include <WiFi.h> 
#include <PubSubClient.h> 
#include <HTTPClient.h>
 
/**************************************** 
 * Define Constants 
 ****************************************/ 
#define WIFISSID "prueba" //WIFI SSID aqui 
#define PASSWORD "33124218741242" // WIFI pwd 
#define TOKEN "BBFF-JqDbdymf5b4ZbqQx74Rr65oOkVcrRx" // Ubidots TOKEN name el mismo que usamos en clase 
#define MQTT_CLIENT_NAME "BBFF-e5517e42984cf447bd726575cab28fdd53a" //ID del cliente, 8 a 12 chars alfanumericos 
//(ASCII), debe ser random y unico dif a otros devices 
 
 
#define VARIABLE_LABEL_temp "temperatura" // Variable Temperatura 
#define VARIABLE_LABEL_hum "humedad" // Variable Humedad 
#define DEVICE_LABEL "sala" // Nombre del dispositivo a crear 
 
char mqttBroker[]  = "industrial.api.ubidots.com"; 
char payload[200]; // Leer y entender el payload aqui una de tantas referencias 
//"https://techterms.com/definition/payload" 
char topic[150]; //Espacio para el nombre del topico 
 
// Space to store values to send 
char str_temp[10]; 
char str_hum[10]; 
 
 
#include "DHT.h" 
#define pin1 13 
DHT dht1(pin1, DHT11);    //El sensor de temp y humedad azul. 
 
/**************************************** 
 * Funciones auxiliares 
 ****************************************/ 
WiFiClient ubidots; 
PubSubClient client(ubidots); 

const char* serverName = "http://industrial.api.ubidots.com/api/v1.6/variables/6345ccfbdec6db048e5835da/values";
 
void callback(char* topic, byte* payload, unsigned int length) { 
  char p[length + 1]; 
  memcpy(p, payload, length); 
  p[length] = NULL; 
  String message(p); 
  Serial.write(payload, length); 
  Serial.println(topic); 
} 
 
void reconnect() { 
  // Loop until we're reconnected 
  while (!client.connected()) { 
    Serial.println("Attempting MQTT connection..."); 
     
    // Attemp to connect 
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) { 
      Serial.println("Connected"); 
    } else { 
      Serial.print("Failed, rc="); 
      Serial.print(client.state()); 
      Serial.println(" try again in 2 seconds"); 
      // Wait 2 seconds before retrying 
      delay(2000); 
    } 
  } 
} 
 
/**************************************** 
 * Main Functions 
 ****************************************/ 
void setup() { 
  Serial.begin(115200); 
  WiFi.begin(WIFISSID, PASSWORD); 
 
   
  Serial.println(); 
  Serial.print("Wait for WiFi..."); 
   
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print("."); 
    delay(500); 
  } 
   
  Serial.println(""); 
  Serial.println("WiFi Connected"); 
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP()); 
  client.setServer(mqttBroker, 1883); 
  client.setCallback(callback);   
 
  // Sensor de temp y humedad 
  dht1.begin(); 
} 
 
void loop() { 

  if(WiFi.status()== WL_CONNECTED){
      WiFiClient client2;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client2, serverName);

       http.addHeader("Content-Type", "application/json");
       http.addHeader("X-Auth-Token", "BBFF-JqDbdymf5b4ZbqQx74Rr65oOkVcrRx");

      // Specify content-type header
      //http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST

      float t1 = dht1.readTemperature(); 
 
      Serial.println(t1); // Imprime temperatura en el serial monitor  
     
      /* numero maximo 4 precision 2 y convierte el valor a string*/ 
      dtostrf(t1, 4, 2, str_temp); 
      
      sprintf(payload, "{\"value\": %s}", str_temp); // formatea el mensaje a publicar 
      //Send HTTP POST request
      int httpResponseCode = http.POST(payload);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
//      int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
     // int httpResponseCode = http.POST("Hello, World!");
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      delay(5000);

      float h1 = dht1.readHumidity();
 
      Serial.println(h1); // Imprime temperatura en el serial monitor  
     
      /* numero maximo 4 precision 2 y convierte el valor a string*/ 
      dtostrf(t1, 4, 2, str_hum); 
      
      sprintf(payload, "{\"value\": %s}", str_hum); // formatea el mensaje a publicar 
         
      //Send HTTP POST request
      httpResponseCode = http.POST(payload);

      Serial.println(http.getString());
        
      // Free resources
      http.end();
    }

  delay(5000);
 
  
   
  
} 
