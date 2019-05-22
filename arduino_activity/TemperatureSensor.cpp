#include "TemperatureSensor.h"

#include "OneWire.h"

byte addr[8];

int digitalPin = 7;
float temperatureCelcius = 0;

TemperatureSensor::TemperatureSensor(int _digitalPin){
  digitalPin = _digitalPin;
}

TemperatureSensor::~TemperatureSensor(){}

void TemperatureSensor::start(){
  OneWire ds(digitalPin);

  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  float celsius;
  
  ds.search(addr);
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  ds.reset_search();
 

  if(addr[0] != 0){

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    //Serial.print(data[i], HEX);
    //Serial.print(" ");
  }
  
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  temperatureCelcius = (float)raw / 16.0;
  //fahrenheit = celsius * 1.8 + 32.0;
    
  } else {
    //Serial.println("Temperature sensor error");
  }
}

float TemperatureSensor::getData(){
  return temperatureCelcius;
}
