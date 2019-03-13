#ifndef WifiSender_H
#define WifiSender_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
class WifiSender{
  public:
    WifiSender();
    void sendDataToServer(WiFiClient wifiClient, String data);
};
#endif
