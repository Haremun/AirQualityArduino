#include "EspConnection.h"

EspConnection::EspConnection(SoftwareSerial * serial, short pin){
    espSerial = serial;
    espPin = pin;
}

void EspConnection::turnOn(){
    digitalWrite(espPin, HIGH);
}
void EspConnection::turnOff(){
    digitalWrite(espPin, LOW);
}

void EspConnection::sendCommandToEsp(byte cmd){
    espSerial->write(cmd);
    espSerial->flush();
}
void EspConnection::sendMessageToEsp(char * message){
    strcat(message, "$");
    espSerial->print(message);
    espSerial->flush();
    memset(message, 0, sizeof(message)); 
}
void EspConnection::waitForEspCommand(byte command){  
    bool waiting = true;
    while(waiting){
      byte cmd = readEspCommand();
      if(cmd == command){
        waiting = false;
      }
      delay(10);
    }
}
byte EspConnection::readEspCommand(){
    espSerial->listen();
    byte command;
    if(espSerial->available() > 0){
      command = espSerial->read();
      //espSerial.flush();      
    } else {
      command = 0x00;        
    }
    return command;
}

EspConnection::~EspConnection(){}
