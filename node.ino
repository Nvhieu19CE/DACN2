
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define SS 10
#define RST 8
#define DIO0 9
#define RLAY 3

//#define RLAY1 5
 
int DHTPIN = 2;//pin where the dht11 is connected
const int DHTTYPE = DHT11;
//DHT dht(DHTPIN, DHTTYPE);
String data = "Subscribe Tech Vegan";
DHT dht(DHTPIN, DHT11);
 
String LoRaMessage = ""; 
 
void setup()
{
  pinMode(A0, INPUT);
  pinMode(RLAY, OUTPUT);
//  pinMode(RLAY1, OUTPUT);
  dht.begin();
  Serial.begin(9600);
  while (!Serial);
  Serial.println(F("LoRa Sender"));
  LoRa.setPins(SS, RST, DIO0);
//  LoRa.setTxPower(TX_P);
//  LoRa.setSyncWord(ENCRYPT);
if (!LoRa.begin(433E6)) {
    Serial.println(F("Starting LoRa failed!"));
    delay(100);
    while (1);
  }
}
 
void loop()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int as = digitalRead(A1);
  int value = analogRead(A0); 
  value = map(value, 0, 1023, 100, 0);

 
  
 // if (value < 25) digitalWrite(3, 1);
 // else digitalWrite(3, 0); 
  if (value < 30) {
    digitalWrite(RLAY, 0);
    Serial.print("On Bom");   
    }
    else {
      digitalWrite(RLAY, 1);
      Serial.print("Off Bom");
      }
  
 // else digitalWrite(3, 0);

  
  Serial.print(F("Temperature = "));
  Serial.print(temperature);
  Serial.println(F("*C"));

//    if (dht.readTemperature() >40 ) {
//    digitalWrite(RLAY1, 0);
//    Serial.print("On QUAT");   
//    }
//    else {
//      digitalWrite(RLAY1, 1);
//      Serial.print("Off QUAT");
//      }
//  Serial.println();
 
  Serial.print(F("Humidity = "));
  Serial.print(humidity);
  Serial.println(F("%"));

  Serial.print(F("Value = "));
  Serial.print(value);
  Serial.println(F("%"));

  Serial.print("as: ");
  Serial.println(digitalRead(A1));

  LoRaMessage =  String(temperature) + "/" + String(humidity) + "&" + String(value) +"@"+ String(as) ;
               
  // send packet
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();
  delay(5000);
}
