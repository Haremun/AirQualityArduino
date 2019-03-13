#ifndef DustSensor_H
#define DustSensor_H

#include <Arduino.h>

class DustSensor{
  public:
    DustSensor(int voPin, int ledPin);
    ~DustSensor();
    void start();
    float getOutput();
};

#endif
