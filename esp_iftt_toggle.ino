#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "EasyMFRC522.h"


 /**
  * ----------------------------------------------------------------------------
  * Easy MFRC522 library - Dictionary View - Example #1
  * (Further information: https://github.com/pablo-sampaio/easy_mfrc522)
  * 
  * -----------------------------------------
  * Minimal example of reading/writing data as key-value pairs, using DictionaryView.
  * 
  * Hardware: you need an Arduino or Esp8266 connected to a MFRC522 reader, and
  * at least one Mifare Classic card/tag.
  * 
  * -----------------------------------------
  * Pin layout used (where * indicates configurable pin):
  * -----------------------------------------
  * MFRC522      Arduino       NodeMCU
  * Reader       Uno           Esp8266
  * Pin          Pin           Pin    
  * -----------------------------------------
  * SDA(SS)      4*            D4*
  * SCK          13            D5   
  * MOSI         11            D7
  * MISO         12            D6
  * RST          3*            D3*
  * NC(IRQ)      not used      not used
  * 3.3V         3.3V          3V
  * GND          GND           GND
  * --------------------------------------------------------------------------
  */

const char* ssid = "HOME2 WIFI 4G";
const char* password = "Home1234";
bool toggle=0;
const char* url1 = "http://maker.ifttt.com/trigger/turnon/with/key/funmKihPUMmzuBkXQdKRNv_uMZDBChvvpptdl7k44fN";
const char* url2 = "http://maker.ifttt.com/trigger/turnoff/with/key/funmKihPUMmzuBkXQdKRNv_uMZDBChvvpptdl7k44fN";
unsigned long lastTime = 0;
int result=0;
unsigned long timerDelay = 10000;
char stringBuffer[100];
int httpResponseCode=400;
EasyMFRC522 rfidReader(D4, D3);
 

void setup() {
  Serial.begin(9600);
   pinMode(13, OUTPUT);
    pinMode(10, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  randomSeed(analogRead(0));
  rfidReader.init(); 
   Serial.println("wave a mifare card");


  
}

void loop() {
    
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;


      if (rfidReader.detectTag())
      {  
        delay(500);
       result = rfidReader.readFile(1, "mylabel", (byte*)stringBuffer, 100);
       Serial.printf("--> String data retrieved: \"%s\" (bytes: %d)\n", stringBuffer, result);
         Serial.println(result);
        if (result >= 0){
         if( strcmp(stringBuffer,"office light toggle")==0)
          toggle=!toggle;
            Serial.println(toggle);
          if(toggle)
         { 
        
          http.begin(client, url1);
          httpResponseCode = http.GET();
           Serial.println("Turning lights on");
           Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
         }
         else
         {
          http.begin(client, url2);
          httpResponseCode = http.GET();
           Serial.println("Turning lights off");
           Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
         }
         digitalWrite(10,HIGH);
         delay(500);
         digitalWrite(10,LOW);
         
          }
          else
          {
           Serial.println("read unsucceful try again");
            digitalWrite(10,HIGH);
         delay(200);
         digitalWrite(10,LOW);
          delay(300);

          digitalWrite(10,HIGH);
         delay(200);
         digitalWrite(10,LOW);
          delay(300);

         digitalWrite(10,HIGH);
         delay(200);
         digitalWrite(10,LOW);
          }
      
 
     rfidReader.unselectMifareTag();
     http.end();
      }
    }
 

    

 
}
