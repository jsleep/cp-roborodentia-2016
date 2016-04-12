#include <QTRSensors.h>
#include <RoboShield.h>
#include <RoboShield_Defines.h>

QTRSensorsRC qtr((unsigned char[]) {31,33,35,37,39,41,43,45}, 8);
RoboShield rs(0);

int ledPin = 22;                 // LED connected to digital pin 13

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pinMode(ledPin, OUTPUT);      // sets the digital pin as output
}

void loop() {
  unsigned int sensors[8];               
  int position = qtr.readLine(sensors);
  for(int i = 0; i < 8; i++){
    Serial.print("Position ");
    Serial.print(i);
    Serial.print(" == ");
    Serial.print(sensors[i]);
    Serial.print(" ");
  }
  Serial.println();
  
}
