#include <RoboShield.h>
#include <RoboShield_Defines.h>

#include <QTRSensors.h>



QTRSensorsRC qtr((unsigned char[]) {31, 33, 35,37,39,41, 43, 45}, 8);
RoboShield rs(0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(rs.buttonPressed() == 1){
    int i;
    rs.lcdPrintf("Calibrating...");
    for (i = 0; i < 125; i++)  // make the calibration take about 5 seconds
    {
      qtr.calibrate();
      delay(20);
    }
    rs.lcdClear();
    rs.lcdPrintf("Finished.");
    Serial.println("Calibrated Minimum");
    for (i = 0; i < 8; i++)  // make the calibration take about 5 seconds
    {
      Serial.print("[");
      Serial.print(i);
      Serial.print("]=");
      Serial.print(qtr.calibratedMinimumOn[i]);
      Serial.print(" ");
      //if(sensors[i] > ON_BLACK)
      //sensor_count++;
    }
    Serial.println();
    Serial.println("Calibrated Maximum");
    for (i = 0; i < 8; i++)  // make the calibration take about 5 seconds
    {
      Serial.print("[");
      Serial.print(i);
      Serial.print("]=");
      Serial.print(qtr.calibratedMaximumOn[i]);
      Serial.print(" ");
      //if(sensors[i] > ON_BLACK)
      //sensor_count++;
    }
    Serial.println();
  }
}
