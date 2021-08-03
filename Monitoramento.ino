#if defined(ESP8266)
#include <ESP8266WiFi.h>        
#else
#include <WiFi.h>
#endif
#include <PubSubClient.h>
#include <DHT.h>
#define INTERVAL 1000
#define SSID "" //nome da sua rede
#define PASSWORD "" // senha da rede
#define MQTT_SERVER "quickstart.messaging.internetofthings.ibmcloud.com"
#define TOPIC_NAME "iot-2/evt/status/fmt/json"

const String QUICK_START = "d:quickstart:arduino:";
const String DEVICE_ID = ""; // id do seu dispositivo
const String CLIENT_ID =  QUICK_START + DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(MQTT_SERVER, 1883, wifiClient);
long lastPublishTime = 0;
DHT dht(14, DHT11);
float Temperatura = 0;
float Umidade = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  setupWiFi();
  connectMQTTServer();
}

void loop() {
    long now = millis();
  if (now - lastPublishTime > INTERVAL) {
    lastPublishTime = now;
    readSensor();
    Serial.print("Publish message: ");
        String msg = createJsonString();
    Serial.println(msg);
    client.publish(TOPIC_NAME, msg.c_str());
  }
}
void setupWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(SSID);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void connectMQTTServer() {
  Serial.println("Connecting to MQTT Server...");
  if (client.connect(CLIENT_ID.c_str())) {
    Serial.println("connected");
  } else {
        Serial.print("error = ");
    Serial.println(client.state());
  }
}

void readSensor(){
  float value;
  value = dht.readTemperature();
  if(!isnan(value)){
    
    Temperatura = value;
  }
    value = dht.readHumidity();
  
  if(!isnan(value)){
        Umidade = value;
  }
}

String createJsonString() {
  String data = "{";
    data+= "\"d\": {";
      data+="\"Temperatura\":";
      data+=String(Temperatura);
      data+=",";
      data+="\"Umidade\":";
      data+=String(Umidade);
    data+="}";
  data+="}";
  return data;
}
