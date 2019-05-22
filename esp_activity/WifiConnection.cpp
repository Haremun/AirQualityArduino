#include "WifiConnection.h"
#include <ESP8266WiFi.h>

//const char* ssid = "Huawei";
//const char* password = "kotylizomaslo";

const char* ssid = "TP-LINK_9752";
const char* password = "26461984";

WiFiClient wifiClient;

WifiConnection::WifiConnection(){}

WiFiClient WifiConnection::getWifiClient(){
  return wifiClient;
}

bool WifiConnection::isConnected(){
  if (WiFi.status() != WL_CONNECTED){
    return false;
  } else {
    return true;
  }
}
int WifiConnection::connect() {
    // Check if we have a WiFi connection, if we don't, connect.
  int xCnt = 0;

  if (WiFi.status() != WL_CONNECTED){
        
        Serial.println();
        Serial.println();
        Serial.print("Connecting to ");
        Serial.println(ssid);

        WiFi.mode(WIFI_OFF);
        delay(1000);
        WiFi.mode(WIFI_STA);
        
        WiFi.begin(ssid, password);
        
        while (WiFi.status() != WL_CONNECTED  && xCnt < 50) {
          delay(500);
          Serial.print(".");
          xCnt ++;
        }

        if (WiFi.status() != WL_CONNECTED){
          Serial.println("WiFiCon=0");
          return 0; //never connected
        } else {
          Serial.println("WiFiCon=1");
          Serial.println("");
          Serial.println("WiFi connected");  
          Serial.println("IP address: ");
          Serial.println(WiFi.localIP());
          return 1; //1 is initial connection
        }

  } else {
    Serial.println("WiFiCon=2");
    return 2; //2 is already connected
  
  }
}
