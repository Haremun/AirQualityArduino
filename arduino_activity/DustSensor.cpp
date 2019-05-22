#include "DustSensor.h"


DustSensor::DustSensor(SoftwareSerial * serial){
    dustSerial = serial;
}

void DustSensor::setupSensor(){
  dustSerial->begin(9600);
}

void DustSensor::start(){
  uint8_t mData = 0;
  uint8_t mPkt[10] = {0};
  uint8_t mCheck = 0;
  while( dustSerial->available() > 0 ) {
    
    for( int i=0; i<10; ++i ) {
      mPkt[i] = dustSerial->read();
    }
    if( 0xC0 == mPkt[1] ) {
      uint8_t sum = 0;
      for( int i=2; i<=7; ++i ) {
        sum += mPkt[i];
      }
      if( sum == mPkt[8] ) {
        uint8_t pm25Low   = mPkt[2];
        uint8_t pm25High  = mPkt[3];
        uint8_t pm10Low   = mPkt[4];
        uint8_t pm10High  = mPkt[5];
  
        Pm25 = ((pm25High * 256.0) + pm25Low) / 10.0;
        Pm10 = ((pm10High * 256.0) + pm10Low) / 10.0;
        
      }
    }
    
    dustSerial->flush();
  }
}

double DustSensor::getPm25(){  
  return Pm25;
}

double DustSensor::getPm10(){  
  return Pm10;
}

DustSensor::~DustSensor(){}
