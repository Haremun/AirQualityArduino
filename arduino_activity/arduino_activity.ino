#include <SoftwareSerial.h>
#include "DustSensor.h"
#include "TemperatureSensor.h"

SoftwareSerial espSerial(10, 11);  //RX,TX
char msg[50];
char sensorValue[10];

DustSensor dustSensor(A0, 7);
TemperatureSensor temperatureSensor(6);

void setup() {
  espSerial.begin(9600);
  Serial.begin(9600);
  delay(100);

  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

  memset(msg, 0, sizeof(msg));
  memset(sensorValue, 0, sizeof(sensorValue));

}

void loop() {
  int i = random(90, 110);
  char ra[3];
  itoa(i, ra, 10);
  
  temperatureSensor.start();
  dtostrf(temperatureSensor.getData(), 3, 2, sensorValue);
  
  char temp[] = "temperature=";    
  strcat(msg, temp);
  strcat(msg, sensorValue);
  memset(sensorValue, 0, sizeof(sensorValue));

  digitalWrite(8, LOW);
  delay(1000); //minuta na załadowanie
  //pętla do obliczenia średniej
  int ariQ_sensor = analogRead(A1);
  int airquality = map(ariQ_sensor, 0, 1023, 0, 255);
  itoa(airquality, sensorValue, 10);
  
  char airq[] = "&airquality=";
  strcat(msg, airq);
  strcat(msg, sensorValue);
  memset(sensorValue, 0, sizeof(sensorValue));

  dustSensor.start();
  dtostrf(dustSensor.getOutput(), 3, 2, sensorValue);

  char dust[] = "&dust=";
  //char end = '$';
  strcat(msg, dust);
  strcat(msg, sensorValue);
  memset(sensorValue, 0, sizeof(sensorValue));
  digitalWrite(8, HIGH);
  
  strcat(msg, "$");  
  espSerial.print(msg);
  Serial.println(msg);
  
  memset(msg, 0, sizeof(msg));
  delay(5000);

}
