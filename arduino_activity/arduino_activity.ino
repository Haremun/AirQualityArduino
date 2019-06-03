#include <SoftwareSerial.h>
#include "DustSensor.h"
#include "TemperatureSensor.h"
#include <EEPROM.h>

const double timeInterval[] = {
  0.5,
  1,
  3,
  5,
  10,
  15,
  30,
  60
}; //minutes

//SoftwareSerial pins
const short dustSerialRX = 12;
const short dustSerialTX = 11;
const short espSerialRX = 3;
const short espSerialTX = 2;
const short bletoothSerialRX = 6;
const short bletoothSerialTX = 5;
//Transistors pins
const short pinBluetooth = 4;
const short pinAirQTrans = 9;
//Other
const short pinEspSwitch = 10;
const short pinBluetoothBtn = 8;
const short pinTemperatureSensor = 7;
//Analog pins
const short analogPinAirQ = 0;
const short analogPinBattery = 1;

const short loopDelay = 10; //miliseconds
short turnOnDelay = 1 * 60; //delay in seconds
short sensorsReadDelay = 1 * 60; //delay in seconds
//short loopCount = sensorsReadDelay * 100;
short loopCount = 0;

bool bluetoothON = false;

const char temp[] = "temperature=";
const char airq[] = "airquality=";
const char dust[] = "dust25=";
const char dust10[] = "dust10=";
const char battery[] ="battery=";
const char test[] = "temperature=11.00&airquality=11.00&battery=4.0&dust25=11.00&dust10=11.00$";

//Declare Serials
SoftwareSerial espSerial(espSerialRX, espSerialTX);  //RX,TX
SoftwareSerial dustSerial(dustSerialRX, dustSerialTX);  //RX,TX
SoftwareSerial bluetoothSerial(bletoothSerialRX, bletoothSerialTX);

//Arrays for data
char msg[100];
char sensorValue[10];
char response[100];
char settingsMessage[100];

//Declare sensors
DustSensor dustSensor(&dustSerial);
TemperatureSensor temperatureSensor(pinTemperatureSensor);

void setup() {

  //analogReference(EXTERNAL);
  
  delay(1000);
  espSerial.begin(9600);  
  bluetoothSerial.begin(9600);
  Serial.begin(9600);
  dustSensor.setupSensor();
  delay(100); 

  byte in = EEPROM.read(0);
  sensorsReadDelay = timeInterval[(int) in] * 60;
    

  pinMode(pinAirQTrans, OUTPUT);
  //pinMode(pinTemperatureSensor, INPUT);
  pinMode(pinBluetoothBtn, INPUT);
  pinMode(pinBluetooth, OUTPUT);
  pinMode(pinEspSwitch, OUTPUT);
  digitalWrite(pinEspSwitch, HIGH);

  Serial.println("Connect to esp:");
  waitForEspCommand(0x01);
  sendCommandToEsp(0x03);
  Serial.println("Connected");
  delay(100);
  digitalWrite(pinEspSwitch, LOW);

  readSensors();
  
  //Reset data arrays
  memset(msg, 0, sizeof(msg));
  memset(sensorValue, 0, sizeof(sensorValue));
  memset(settingsMessage, 0, sizeof(settingsMessage));

  digitalWrite(pinAirQTrans, LOW);
  dustSensor.sensorSleep();
  delay(100); 
}

void loop() { 
  checkBluetoothBtnStatus();
  
  //Bluetooth mode--------------------------------
  if(bluetoothON){
    if(bluetoothSerial.available() > 0){      
      short msgCounter = 0;
      short seperatorCount = 0;

      while(bluetoothSerial.available() > 0){
        char letter = bluetoothSerial.read();
        if(letter == '&'){
          seperatorCount++;
        }
        if(seperatorCount != 2){
          settingsMessage[msgCounter] = letter;
          msgCounter++;
        } 
        else {
          char index_char[1]; index_char[0] = bluetoothSerial.read();
          Serial.println(index_char);
          int index = atoi(index_char);
          byte b = (byte)index; //saving index to byte value for EEPROM
          EEPROM.write(0, b);
          sensorsReadDelay = timeInterval[index] * 60;
          Serial.println(sensorsReadDelay);
          //memset(settingsMessage, 0, sizeof(settingsMessage));
        }       
        
      }
    }
    
    
    
    if((loopCount * loopDelay)/1000 > turnOnDelay){
      digitalWrite(pinAirQTrans, HIGH);     
    }
    if((loopCount * loopDelay)/1000 > sensorsReadDelay){
    
      readSensors();    

      strcat(msg, "\n");
      bluetoothSerial.print(msg);
    
      Serial.println(msg);
      bluetoothSerial.listen();
  
      memset(msg, 0, sizeof(msg));
      loopCount = 0;
      digitalWrite(pinAirQTrans, LOW);
      } else {
        loopCount++;
      }
  } else { //WiFi mode--------------------------------

      if((loopCount * loopDelay)/1000 > turnOnDelay){
      digitalWrite(pinAirQTrans, HIGH);     
      }
      if((loopCount * loopDelay)/1000 > sensorsReadDelay){
        
        readSensors();
        digitalWrite(pinAirQTrans, LOW);

        digitalWrite(pinEspSwitch, HIGH);
        Serial.println("Wait for 0x01:"); 
        waitForEspCommand(0x01);

        delay(100);
        sendCommandToEsp(0x02);
        delay(1000);
        sendMessageToEsp(msg);

        Serial.println("Wait for msg send:"); 
        waitForEspCommand(0x09);        
        Serial.println("Done"); 
        delay(3000);
        
        digitalWrite(pinEspSwitch, LOW);
        delay(5000);
        loopCount = 0;  
      }else {
        loopCount++;
      }    
      
  
                
    }   
        
  delay(loopDelay);
    
}

//Functions**************************************************************************************************************************

  void waitForEspCommand(byte command){  
    bool waiting = true;
    int o = 0;
    while(waiting){
      byte cmd = readEspCommand();
      if(cmd == command){
        waiting = false;
        Serial.println(cmd, HEX);
        double wynik = (o * 10) / 1000;
        Serial.println(o);
      }
      o++;
      delay(10);
    }
  }
  void sendCommandToEsp(byte command){    
    espSerial.write(command);
    espSerial.flush();    //delay niepotrzebny  
  }
  void sendMessageToEsp(char * message){    
    strcat(message, "$");
    espSerial.print(message);
    espSerial.flush();
    memset(message, 0, sizeof(message)); 
  }

  byte readEspCommand(){
    espSerial.listen();
    byte command;
    if(espSerial.available() > 0){
      command = espSerial.read();
      //espSerial.flush();      
    } else {
      command = 0x00;        
    }
    return command;
  }
  //-------------------------------------------------------
  //Reading sensors values
  //-------------------------------------------------------
  void readSensors(){
    
      temperatureSensor.start();        
      saveValue(temp, temperatureSensor.getData());    
      
      float ariQ_sensor = analogRead(analogPinAirQ);
      float airquality = map(ariQ_sensor, 0, 1023, 0, 255);
      //itoa(airquality, sensorValue, 10);
      saveValue(airq, airquality);      

      dustSerial.listen();
      delay(100);
      dustSensor.sensorWakeup();
      delay(3000);
      dustSensor.sensorRead();        
      saveValue(dust, dustSensor.getPm25()); 
      saveValue(dust10, dustSensor.getPm10());
      dustSensor.sensorSleep();

      float battery_sensor = 0;
      float sum = 0;
      int avLimit = 200;
      for(int i = 0; i < avLimit; i++){
        battery_sensor = analogRead(analogPinBattery);
        sum += battery_sensor;
      }
      //1024 is max value for analogRead, 1024 = 5.05V
      float wolty = (sum/avLimit) / 202.78; // przekształcam wartość na wolty
      saveValue(battery, wolty, true);
  }

  //-------------------------------------------------------
  //Saving value
  //-------------------------------------------------------
  void saveValue(const char * sensor, float value){
    dtostrf(value, 3, 1, sensorValue);
    if(!bluetoothON){
      strcat(msg, sensor);
    }
      strcat(msg, sensorValue);
      strcat(msg, "&");
      memset(sensorValue, 0, sizeof(sensorValue));  
  }
  void saveValue(const char * sensor, float value, bool last){
    dtostrf(value, 3, 1, sensorValue);
    if(!bluetoothON){
      strcat(msg, sensor);
    }
      strcat(msg, sensorValue);
      memset(sensorValue, 0, sizeof(sensorValue));  
  }
  //-------------------------------------------------------
  //Bluetooth status
  //-------------------------------------------------------
  void checkBluetoothBtnStatus(){
    
    int bluetooth = digitalRead(pinBluetoothBtn);
    if(bluetooth == HIGH){
      Serial.println("on");
      delay(500);
      if(!bluetoothON){
        digitalWrite(pinBluetooth, HIGH);
        bluetoothSerial.listen();
        bluetoothON = true;
      } else {
        digitalWrite(pinBluetooth, LOW);
        espSerial.listen();
        bluetoothON = false;
      }    
    }
  }
