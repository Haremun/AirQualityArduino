#ifndef DustSensor_H
#define DustSensor_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class DustSensor{
  private:
    SoftwareSerial * dustSerial;
    double Pm25 = 0;
    double Pm10 = 1;
    void writeToSensor(const byte * cmd);
    
  public:
    DustSensor(SoftwareSerial * serial);
    ~DustSensor();
    void setupSensor();
    void sensorRead();
    void sensorSleep();
    void sensorWakeup();
    double getPm25();
    double getPm10();
};

#endif
