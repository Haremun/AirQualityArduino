#ifndef EspConnection_H
#define EspConnection_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class EspConnection{
  private:
    SoftwareSerial * espSerial;
    short espPin;
    
  public:
    EspConnection(SoftwareSerial * serial, short pin);
    ~EspConnection();
    void turnOn();
    void turnOff();
    void sendCommandToEsp(byte cmd);
    void sendMessageToEsp(char * message);
    byte readEspCommand();
    void waitForEspCommand(byte command);
};

#endif
