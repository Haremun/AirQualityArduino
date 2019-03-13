#include "DustSensor.h"

#ifdef USE_AVG
#define N 100
static unsigned long VoRawTotal = 0;
static int VoRawCount = 0;
#endif // USE_AVG

float dustDensity;

int sharpLEDPin = 7;
int sharpVoPin = A0;

// Set the typical output voltage in Volts when there is zero dust. 
const float Voc = 0.02;

// Use the typical sensitivity in units of V per 100ug/m3.
const float K = 0.5;

DustSensor::DustSensor(int voPin, int ledPin){
  sharpVoPin = voPin;
  sharpLEDPin = ledPin;

  pinMode(sharpLEDPin, OUTPUT);
}

void DustSensor::start(){
  digitalWrite(sharpLEDPin, LOW);

  // Wait 0.28ms before taking a reading of the output voltage as per spec.
  delayMicroseconds(280);
  //delayMicroseconds(450);

  // Record the output voltage. This operation takes around 100 microseconds.
  int VoRaw = analogRead(sharpVoPin);
  // Turn the dust sensor LED off by setting digital pin HIGH.
  digitalWrite(sharpLEDPin, HIGH);
  
  delayMicroseconds(9620);

  float Vo = VoRaw;
  #ifdef USE_AVG
  bool avgDone = false;
  while(!avgDone){
    VoRawTotal += VoRaw;
    VoRawCount++;
    if ( VoRawCount >= N ) {
      Vo = 1.0 * VoRawTotal / N;
      Serial.print(Vo);
      VoRawCount = 0;
      VoRawTotal = 0;
    } else {
      avgDone = true;
    }
  }  
  #endif // USE_AVG

  Vo = Vo / 1024.0 * 5.0;

  float dV = Vo - Voc;
  if ( dV < 0 ) {
    dV = 0;
    //Voc = Vo;
  }
  dustDensity = dV / K * 100.0;
}


float DustSensor::getOutput(){  
  return dustDensity;
}

DustSensor::~DustSensor(){}
