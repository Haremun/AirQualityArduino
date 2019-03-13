#ifndef TemperatureSensor_H
#define TemperatureSensor_H

#include <Arduino.h>

class TemperatureSensor{
  public:
    TemperatureSensor(int);
    ~TemperatureSensor();
    void start();
    float getData();
};

#endif
