#include "WifiSender.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


HTTPClient http;

WifiSender::WifiSender(){}

void WifiSender::sendDataToServer(WiFiClient wifiClient, String data){
    http.begin(wifiClient, "http://esp8266collection.keep.pl/json/postman.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int i = http.POST(data);
    Serial.flush();
    Serial.begin(9600);
    //delay(1000);    
    //Serial.print("HTTP Code: "); Serial.println(i);
    //Serial.println(http.getString());
    http.end();
  }
