  #include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define SS 15
#define RST 16
#define DIO0 2

#define FIREBASE_AUTH "ADhz6uK6a7LHCZtKmURgpi6EV8mpf1kZQxXWzkLi"
#define API_KEY "AIzaSyA8iSw1eKuetxTTUnuo7rJq1E7PnON1j9Y"
#define WIFI_SSID "Hieu"
#define WIFI_PASSWORD "123321111"


/* 3. Define the RTDB URL */
#define DATABASE_URL "https://lora-nn-111f3-default-rtdb.asia-southeast1.firebasedatabase.app//" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "hieu32540@gmail.com"
#define USER_PASSWORD "20022001Hieu"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

String temperature;
String humidity;
String as;
String value;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Receiver");
  
//firebase  
  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
 //
  
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  } 
  // register the receive callback

  // put the radio into receive mode
  LoRa.receive();
}

void loop() {
  int pos1, pos2, pos3;
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet: ");
    String LoRaData = LoRa.readString();
    Serial.print(LoRaData);
    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    pos1 = LoRaData.indexOf('/');
    pos2 = LoRaData.indexOf('&');
    pos3 = LoRaData.indexOf('@');

    temperature = LoRaData.substring(0, pos1);
    humidity = LoRaData.substring(pos1 + 1, pos2);
    value = LoRaData.substring(pos2 + 1, pos3);
    as = LoRaData.substring(pos3 + 1, LoRaData.length());

    Firebase.setString(fbdo, F("/Sensor/temperature"), temperature);
    Firebase.setString(fbdo, F("/Sensor/humidity"), humidity);
    Firebase.setString(fbdo, F("/Sensor/value"), value);
    Firebase.setString(fbdo, F("/Sensor/as"), as);
    
    Serial.print(F("Temperature = "));
    Serial.print(temperature);
    Serial.println(F("*C"));
  
    Serial.print(F("Humidity = "));
    Serial.print(humidity);
    Serial.println(F("%"));

    Serial.print(F("Soil moisure = "));
    Serial.print(value);
    Serial.println(F(" %"));
    
    Serial.print(F("Light = "));
    Serial.print(as);
    Serial.println(F(" lx"));


    Serial.println();

  }
}
