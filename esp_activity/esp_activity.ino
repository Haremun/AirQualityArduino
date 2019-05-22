#include "WifiConnection.h"
#include "WifiSender.h"

WifiConnection wiFiConn;
WifiSender sender;

char myArray[60];
int i = 0;
int length = 0;
bool sendMsg = false;
bool newMessage = false;

void setup() {
  
  Serial.begin(115200);
  wiFiConn.connect();

  Serial.flush();
  delay(1000);
  Serial.begin(9600);  

  memset(myArray, 0, sizeof(myArray));

  /*if(wiFiConn.isConnected()){
       sender.sendDataToServer(wiFiConn.getWifiClient(), "temperature=12&airquality=19&dust=05&");
      }*/
}

void loop() {
  
  if(Serial.available() > 0){
    Serial.print("Nope");
    while(!sendMsg){
      char letter = Serial.read();
          
      if(letter != '$'){      
        myArray[i] = letter;
        i++;
      } else {
        sendMsg = true;
        Serial.print(myArray);
      }
      delay(30);
    }
    if(sendMsg){
      sendMsg = false;
      Serial.print(myArray);
      delay(1000);
      Serial.flush();
      Serial.begin(115200);
      delay(1000);
      if(wiFiConn.isConnected()){
       sender.sendDataToServer(wiFiConn.getWifiClient(), myArray);
      }
      delay(2000);
      i = 0;
      length = 0;
      memset(myArray, 0, sizeof(myArray));
    }        
  }
  /*if(wiFiConn.isConnected()){
       sender.sendDataToServer(wiFiConn.getWifiClient(), "temperature=13&airquality=19&dust=05&");
    }*/
    delay(2000);
}
