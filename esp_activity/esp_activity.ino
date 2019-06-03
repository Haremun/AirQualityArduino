#include "WifiConnection.h"
#include "WifiSender.h"

WifiConnection wiFiConn;
WifiSender sender;

char myArray[100];
int i = 0;
int limit = 0;
bool sendMsg = false;

const static byte CMD_TURN_ON = 0x01;
const static byte CMD_SEND = 0x02;

void setup() {
  
  Serial.begin(115200);
  wiFiConn.connect();

  Serial.flush();
  delay(1000);
  Serial.begin(9600); 

  gpio_pin_wakeup_enable(GPIO_ID_PIN(2), GPIO_PIN_INTR_HILEVEL);
  wifi_set_sleep_type(LIGHT_SLEEP_T);  

  memset(myArray, 0, sizeof(myArray));
  //strcat(myArray, "temperature=0.00&airquality=147.00&battery=2.33&dust25=0.00&dust10=1.00");
  //sendm();
  delay(2000);
}

void loop() {
  delay(500);
  limit = 0;
  sendCommand(0x01);
    bool wait = true;
    while(wait && limit < 1000){ //nie ptrzetestowane
      if(Serial.available() > 0){
        byte cmd = Serial.read();
        if(cmd == 0x02){
          //sendMsg = false;
          while(!sendMsg){
            readMsg();
          }
          sendMsg = false;
          wait = false;              
        }
        if(cmd == 0x03){
          wait = false;
        }
      }
      limit++;
      delay(10);
    }  
  if(limit < 1000){
    delay(5000); //mniejszy
  }  

}
void sendCommand(byte cmd){
  Serial.write(cmd);
  Serial.flush();
}

void readMsg(){
  while(Serial.available() > 0 && !sendMsg){
      char letter = Serial.read();
          
      if(letter != '$'){      
        myArray[i] = letter;
        i++;
      } else {
        i = 0;
        sendMsg = true;
        sendm();
      }
    }
}
void sendm(){
  //sendMsg = false;
  //delay(1000);
  //Serial.flush();
  Serial.begin(115200);
  //delay(1000);
  if(wiFiConn.isConnected()){
   sender.sendDataToServer(wiFiConn.getWifiClient(), myArray);
   Serial.write(0x09);
  }
  //delay(2000);
  i = 0;
  memset(myArray, 0, sizeof(myArray));
}
