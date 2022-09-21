

#include "WiFi.h"
#include "time.h"
#include "DHT.h"

#define pin1 27
#define pin2 26

DHT dht1(pin1, DHT11);

const char* ssid     = "lulucarstensen";
const char* password = "19932000";
int pot_value = 0;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

void setup(){
  Serial.begin(115200);
  dht1.begin();
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  pinMode(pin2,INPUT);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
 // WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
}

void loop(){
  delay(3000);
  printLocalTime();
  leerdht1();
  pot_value = digitalRead(pin2);
  Serial.println("Pot Value: ");
  Serial.println(pot_value);
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println(WiFi.localIP()); 
  Serial.println();
}

void leerdht1() {
   float t1 = dht1.readTemperature();
   float h1 = dht1.readHumidity();
   while (isnan(t1) || isnan(h1)) {
     Serial.println("Lectura fallida en el sensor DHT11, repitiendo lectura...");
     delay(3000); // esperar 3 seg
     t1 = dht1.readTemperature();
     h1 = dht1.readHumidity();
   }
  
   Serial.print("Temperatura DHT11: ");
   Serial.print(t1);
   Serial.println(" ºC.");
   Serial.print("Humedad DHT11: ");
   Serial.print(h1);
   Serial.println(" %.");
   Serial.println("-----------------------");
}
