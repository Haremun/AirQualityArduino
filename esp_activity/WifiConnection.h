#ifndef WifiConnection_H
#define WifiConnection_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

class WifiConnection{
  public:
    WifiConnection();
    int connect();
    bool isConnected();
    WiFiClient getWifiClient();
};

#endif
