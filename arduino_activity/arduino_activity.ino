#include <SoftwareSerial.h>
#include "DustSensor.h"
#include "TemperatureSensor.h"

//SoftwareSerial pins
const short dustSerialRX = 12;
const short dustSerialTX = 11;
const short espSerialRX = 3;
const short espSerialTX = 2;
const short bletoothSerialRX = 5;
const short bletoothSerialTX = 6;
//Transistors pins
const short pinBluetooth = 4;
const short pinAirQTrans = 9;
//Other
const short pinBluetoothBtn = 8;
const short pinTemperatureSensor = 7;
const short analogPinAirQ = 0;

const short loopDelay = 500; //miliseconds
const short sensorsReadDelay = 10; //delay in seconds

bool bluetoothON = false;

//Declare Serials
SoftwareSerial espSerial(espSerialRX, espSerialTX);  //RX,TX
SoftwareSerial dustSerial(dustSerialRX, dustSerialTX);  //RX,TX
SoftwareSerial bluetoothSerial(bletoothSerialRX, bletoothSerialTX);

//Arrays for data
char msg[60];
char sensorValue[10];

//Declare sensors
DustSensor dustSensor(&dustSerial);
TemperatureSensor temperatureSensor(pinTemperatureSensor);

void setup() {
  
  delay(1000);
  espSerial.begin(9600);  
  Serial.begin(9600);
  dustSensor.setupSensor();
  delay(100);

  pinMode(pinAirQTrans, OUTPUT);
  pinMode(pinTemperatureSensor, INPUT);
  pinMode(pinBluetoothBtn, INPUT);
  pinMode(pinBluetooth, OUTPUT);

  dustSensor.start();
  
  //Reset data arrays
  memset(msg, 0, sizeof(msg));
  memset(sensorValue, 0, sizeof(sensorValue));

  //delay(15000);
}

void loop() { 
  
  temperatureSensor.start();
  dtostrf(temperatureSensor.getData(), 3, 2, sensorValue);
  char temp[] = "temperature=";    
  strcat(msg, temp);
  strcat(msg, sensorValue);
  memset(sensorValue, 0, sizeof(sensorValue));

  digitalWrite(pinAirQTrans, HIGH);
  delay(2000); //minuta na załadowanie
  int ariQ_sensor = analogRead(analogPinAirQ);
  int airquality = map(ariQ_sensor, 0, 1023, 0, 255);
  itoa(airquality, sensorValue, 10);
  //digitalWrite(pinAirQTrans, LOW);
  
  char airq[] = "&airquality=";
  strcat(msg, airq);
  strcat(msg, sensorValue);
  memset(sensorValue, 0, sizeof(sensorValue));
  

  dustSensor.start();
  
  dtostrf(dustSensor.getPm25(), 3, 0, sensorValue); 
  char dust[] = "&dust25=";
  strcat(msg, dust);
  strcat(msg, sensorValue);
  memset(sensorValue, 0, sizeof(sensorValue));

  dtostrf(dustSensor.getPm10(), 3, 0, sensorValue);  
  char dust10[] = "&dust10=";
  strcat(msg, dust10);
  strcat(msg, sensorValue);
  memset(sensorValue, 0, sizeof(sensorValue));
  
  
  strcat(msg, "$");  

  espSerial.print(msg);
  Serial.println(msg);
  
  memset(msg, 0, sizeof(msg));
  delay(loopDelay);

  /*int bluetooth = digitalRead(pinBluetoothBtn);
  if(bluetooth == HIGH){
    Serial.println("on");
    delay(500);
    if(!bluetoothON){
      digitalWrite(pinBluetooth, HIGH);
      bluetoothON = true;
    } else {
      digitalWrite(pinBluetooth, LOW);
      bluetoothON = false;
    }    
  }*/
}

/*void ProcessSerialData2()
{
  uint8_t mData = 0;
  uint8_t i = 0;
  uint8_t mPkt[10] = {0};
  uint8_t mCheck = 0;
 while (dustSerial.available() > 0) 
  {  
    Serial.println("Reading");
    // from www.inovafitness.com
    // packet format: AA C0 PM25_Low PM25_High PM10_Low PM10_High 0 0 CRC AB
     mData = dustSerial.read();     delay(2);//wait until packet is received
    if(mData == 0xAA)//head1 ok
     {
        mPkt[0] =  mData;
        mData = dustSerial.read();
        if(mData == 0xc0)//head2 ok
        {
          mPkt[1] =  mData;
          mCheck = 0;
          for(i=0;i < 6;i++)//data recv and crc calc
          {
             mPkt[i+2] = dustSerial.read();
             delay(2);
             mCheck += mPkt[i+2];
          }
          mPkt[8] = dustSerial.read();
          delay(1);
    mPkt[9] = dustSerial.read();
          if(mCheck == mPkt[8])//crc ok
          {
            dustSerial.flush();
            //Serial.write(mPkt,10);

            Pm25 = (uint16_t)mPkt[2] | (uint16_t)(mPkt[3]<<8);
            Pm10 = (uint16_t)mPkt[4] | (uint16_t)(mPkt[5]<<8);
            if(Pm25 > 9999)
             Pm25 = 9999;
            if(Pm10 > 9999)
             Pm10 = 9999;            
            //get one good packet
             return;
          }
        }      
     }
   } 
}
void ProcessSerialData()
{
 uint8_t mData = 0;
  uint8_t mPkt[10] = {0};
  uint8_t mCheck = 0;
  while( dustSerial.available() > 0 ) {
    
    for( int i=0; i<10; ++i ) {
      mPkt[i] = dustSerial.read();
    }
    if( 0xC0 == mPkt[1] ) {
      // Read dust density.
      // Check
      uint8_t sum = 0;
      for( int i=2; i<=7; ++i ) {
        sum += mPkt[i];
      }
      if( sum == mPkt[8] ) {
        uint8_t pm25Low   = mPkt[2];
        uint8_t pm25High  = mPkt[3];
        uint8_t pm10Low   = mPkt[4];
        uint8_t pm10High  = mPkt[5];
  
        Pm25 = ( ( pm25High * 256.0 ) + pm25Low ) / 10.0;
        Pm10 = ( ( pm10High * 256.0 ) + pm10Low ) / 10.0;
        
      }
    }
    
    dustSerial.flush();
  }
  //delay( 1000 );
}*/
