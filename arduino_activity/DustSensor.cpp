#include "DustSensor.h"

static const byte CMD_SLEEP[19] = {
  0xAA, // head
  0xB4, // command id
  0x06, // data byte 1
  0x01, // data byte 2 (set mode)
  0x00, // data byte 3 (sleep)
  0x00, // data byte 4
  0x00, // data byte 5
  0x00, // data byte 6
  0x00, // data byte 7
  0x00, // data byte 8
  0x00, // data byte 9
  0x00, // data byte 10
  0x00, // data byte 11
  0x00, // data byte 12
  0x00, // data byte 13
  0xFF, // data byte 14 (device id byte 1)
  0xFF, // data byte 15 (device id byte 2)
  0x05, // checksum
  0xAB  // tail
};
static const byte CMD_WAKEUP[19] = {
  0xAA, // head
  0xB4, // command id
  0x06, // data byte 1
  0x01, // data byte 2 (set mode)
  0x01, // data byte 3 (sleep)
  0x00, // data byte 4
  0x00, // data byte 5
  0x00, // data byte 6
  0x00, // data byte 7
  0x00, // data byte 8
  0x00, // data byte 9
  0x00, // data byte 10
  0x00, // data byte 11
  0x00, // data byte 12
  0x00, // data byte 13
  0xFF, // data byte 14 (device id byte 1)
  0xFF, // data byte 15 (device id byte 2)
  0x06, // checksum
  0xAB  // tail
};


DustSensor::DustSensor(SoftwareSerial * serial){
    dustSerial = serial;
}

void DustSensor::setupSensor(){
  dustSerial->begin(9600);
}

void DustSensor::sensorRead(){
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
        Serial.println("Dust sensor: ");
        Serial.print(Pm25);
        Serial.print(" ");
        Serial.println(Pm10);
        
      }
    } else {
      Serial.println("Dust sensor error, no 0xC0");
    }
    
    dustSerial->flush();
  }
}

void DustSensor::sensorSleep(){  
  writeToSensor(CMD_SLEEP);
}
void DustSensor::sensorWakeup(){  
  writeToSensor(CMD_WAKEUP);
}

void DustSensor::writeToSensor(const byte * cmd){
  for(uint8_t i = 0; i < 19; i++){
    dustSerial->write(cmd[i]);
  }
  dustSerial->flush();
  while(dustSerial->available() > 0){
    dustSerial->read();
  }  
}

double DustSensor::getPm25(){  
  return Pm25;
}

double DustSensor::getPm10(){  
  return Pm10;
}

DustSensor::~DustSensor(){}
