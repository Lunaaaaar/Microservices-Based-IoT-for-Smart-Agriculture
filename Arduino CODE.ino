#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN D8
#define DHTTYPE DHT11
#define SensorPin A0
DHT dht(DHTPIN, DHTTYPE);


// Update these with values suitable for your network.
const char* ssid = "LunarHotspot";
const char* password = "Avial1998#";
const char* mqtt_server = "10.168.12.11";

WiFiClient espClient;
PubSubClient client(espClient);

const char* channel = "sensors";
const int motorPin = D0;

// Create variable to hold mqtt messages
#define MSG_BUFFER_SIZE  (100)
char msg[MSG_BUFFER_SIZE];

// Connecting to the WIFI network
void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }



  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str(), "mqtt", "")) {
      Serial.println("connected");

      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");

      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW); // keep motor off initally
  dht.begin();

  setup_wifi();

  client.setServer(mqtt_server, 1883);


}

void loop() {

  // Connect to the mqtt client
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Get the current object temperatur of the sensor
  float objt = dht.readTemperature();
  float objt1 = dht.readHumidity();
  float sensorValue = analogRead(SensorPin);


  int moisturePercentage = (100.00 - ((analogRead(SensorPin) / 1023.00) * 100.00));

  if (moisturePercentage < 20) {
    digitalWrite(motorPin, HIGH);
  }
  if (moisturePercentage > 20 && moisturePercentage < 25) {
    digitalWrite(motorPin, HIGH);
  }
  if (moisturePercentage > 26) {
    digitalWrite(motorPin, LOW);
  }


  Serial.print("|Temperature is: ");
  Serial.print(objt);
  Serial.print("|Soil Humidity is: ");
  Serial.print(objt1);
  Serial.print("Soil Moisture is  = ");
  Serial.print(moisturePercentage);
  Serial.println("%");

  // Create the message that will be send using mqtt


  String v2 = ("weather,location=Field1 temperature=" + String(objt));
  client.publish(channel, v2.c_str(), true);
  v2 = ("weather,location=Field1 Humidity=" + String(objt1));
  client.publish(channel, v2.c_str(), true);
  String v1 = ("weather,location=Field1 Moisture=" + String(moisturePercentage));
  client.publish(channel, v1.c_str(), true);

  delay(1000);

}
