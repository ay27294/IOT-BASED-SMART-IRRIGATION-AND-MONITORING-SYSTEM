#include <ESP8266WiFi.h>
#include <SimpleDHT.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
//#include <Watersensor.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#define Relay1 D1
#define WLAN_SSID "Radmi Note 10" // Your SSID
#define WLAN_PASS "bakloldost" // Your password
/* Adafruit.io Setup */
#define AIO_SERVER "io.adafruit.com" //Adafruit Server
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "shivvamm" // Username
#define AIO_KEY "aio_nveA29JFPBp8eZ22YRztUC5TsJ70" // Auth Key
//WIFI CLIENT
WiFiClient client;�
Page | 43
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT,
AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Temperature1 = Adafruit_MQTT_Publish(&mqtt,
AIO_USERNAME "/feeds/Temperature1");
Adafruit_MQTT_Publish Humidity1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME
"/feeds/Humidity1");
Adafruit_MQTT_Publish Soil = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME
"/feeds/Soil");
Adafruit_MQTT_Publish Rain = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME
"/feeds/Rain");
//Adafruit_MQTT_Subscribe buzzer = Adafruit_MQTT_Subscribe(&mqtt,
AIO_USERNAME"/feeds/buzzer");
Adafruit_MQTT_Subscribe moter = Adafruit_MQTT_Subscribe(&mqtt,
AIO_USERNAME"/feeds/moter"); // Feeds name should be same everywhere
int pinDHT11 = 0;//d3
//rain
int sensorPin = D0; // input for LDR and rain sensor
int enable2 = 13; // D7 enable reading Rain indicator
int sensorValue2 = 0; // variable to store the value coming from sensor Rain sensor
SimpleDHT11 dht11(pinDHT11);
byte hum = 0; //Stores humidity value
byte temp = 0; //Stores temperature value
void MQTT_connect();
void setup() {
 Serial.begin(115200);
 pinMode(Relay1, OUTPUT);
 pinMode(enable2, OUTPUT);
//soil
 pinMode(D0,INPUT);
// display.init();
// display.flipScreenVertically();�
Page | 44
// display.setFont(ArialMT_Plain_10);

 Serial.println("€SmartIrrigation system");
// Connect to WiFi access point.
 Serial.println(); Serial.println();
 Serial.print("Connecting to ");
 Serial.println(WLAN_SSID);
 WiFi.begin(WLAN_SSID, WLAN_PASS);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print("..");
 }
 Serial.println();
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
 mqtt.subscribe(&moter);

}
void connect() {
 Serial.print(F("Connecting to Adafruit IO... "));
 int8_t ret;
 while ((ret = mqtt.connect()) != 0) {
 switch (ret) {
 case 1: Serial.println(F("Wrong protocol")); break;
 case 2: Serial.println(F("ID rejected")); break;
 case 3: Serial.println(F("Server unavail")); break;
 case 4: Serial.println(F("Bad user/pass")); break;
 case 5: Serial.println(F("Not authed")); break;
 case 6: Serial.println(F("Failed to subscribe")); break;
 default: Serial.println(F("Connection failed")); break;
 }
if(ret >= 0)�
Page | 45
 mqtt.disconnect();
Serial.println(F("Retrying connection..."));
 delay(3000);
 }
 Serial.println(F("Adafruit IO Connected!"));
}
void loop() {
 MQTT_connect();
 if(! mqtt.ping(3)) {
 // reconnect to adafruit io
 if(! mqtt.connected())
 connect();
 }
//moter subscription
 Adafruit_MQTT_Subscribe *subscription;
 if((subscription = mqtt.readSubscription(5000))) {
 if (subscription == &moter) {
 Serial.print(F("Got: "));
 Serial.println((char *)moter.lastread);
 int moter_State = atoi((char *)moter.lastread);
 digitalWrite(Relay1, moter_State);

 }
 }

//rain sensor
//int soil = digitalRead(D0);//D0
int moisture_percentage;
moisture_percentage = (100.00 - ((analogRead(A0)/1023.00)*100.00));
sensorValue2 = digitalRead(sensorPin);
//sensorValue2 = constrain(sensorValue2, 150, 440); �
Page | 46
//sensorValue2 = map(sensorValue2, 150, 440, 1023, 0);
//Serial.println("sensorvalue");
//Serial.println(sensorValue2);
if (sensorValue2==0){
 Serial.println("rain is detected");
 digitalWrite(enable2, HIGH);
 Rain.publish("1");
 delay(2095);

 }
 else{
 Serial.println("rain not detected");
 digitalWrite(enable2, LOW);
 Rain.publish("0");
 delay(2067);
 }
 Soil.publish(moisture_percentage);
delay(2093);
Serial.print("soil value: ");
Serial.println(moisture_percentage);
if (! Temperature1.publish(temp)) { //Publish to Adafruit
 Serial.println(F("Failed"));
 }
 if (! Humidity1.publish(hum)) { //Publish to Adafruit
 Serial.println(F("Failed"));
 }
 else {
 Serial.println(F("temp hum value Sent!"));
 }
 delay(2037);
dht11.read(&temp, &hum, NULL);
 Serial.print((int)temp); Serial.print(" *C, "); �
Page | 47
 Serial.print((int)hum); Serial.println(" H");
 Serial.println(" ");
 Serial.println(" ");
 Serial.println(" ");
delay(5000);

}
void MQTT_connect() {
 int8_t ret;
 if (mqtt.connected()) {
 return;
 }
 Serial.print("Connecting to MQTT... ");
 uint8_t retries = 3;
 while ((ret = mqtt.connect()) != 0) {
 Serial.println(mqtt.connectErrorString(ret));
 Serial.println("Retrying MQTT connection in 5 seconds...");
 mqtt.disconnect();
 delay(3000);
 retries--;
 if (retries == 0) {
 while (1);
 }
 }
 Serial.println("MQTT Connected!");

}
