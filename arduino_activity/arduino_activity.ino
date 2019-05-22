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

const short loopDelay = 10; //miliseconds
const short sensorsReadDelay = 10; //delay in seconds
short loopCount = 0;

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

  checkBluetoothBtnStatus();

  if(loopCount * loopDelay == sensorsReadDelay * 1000){
    Serial.println(loopCount * loopDelay);
    readSensors();
    strcat(msg, "$");  

    espSerial.print(msg);
    Serial.println(msg);
  
    memset(msg, 0, sizeof(msg));
    loopCount = 0;
  } else {
    loopCount++;
  }
    
  delay(loopDelay);  
}

//*****************************************************************************************

void readSensors(){
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
}
void checkBluetoothBtnStatus(){
  
  int bluetooth = digitalRead(pinBluetoothBtn);
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
  }
}
