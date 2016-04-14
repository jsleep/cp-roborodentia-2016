#include <QTRSensors.h>
#include <RoboShield.h>
#include <RoboShield_Defines.h>
#include <Wire.h>
#define rightMotor 2
#define leftMotor 3
#define ONBLACK 250
#define SENSOR_COUNT 8

#define ON_RIGHT 500
#define SMOOTH_RIGHT 1500
#define ON_LEFT -500
#define SMOOTH_LEFT -1500


QTRSensorsRC qtr((unsigned char[]) {31, 33, 35,37,39,41, 43, 45}, 8);
RoboShield rs(0);

int ledPin = 22;                 // LED connected to digital pin 13

void setup() {
  if (rs.buttonPressed()) {
    rs.debuggingMode();
  }

  
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pinMode(ledPin, OUTPUT);      // sets the digital pin as output

  int i;
  rs.lcdPrintf("Calibrating...");
  /*for (i = 0; i < 125; i++)  // make the calibration take about 5 seconds
  {
    qtr.calibrate();
    delay(20);
  }
  rs.lcdClear();*/
  rs.lcdPrintf("Finished.");
}

void FollowLine(unsigned int distance, int position) {
  
  int error = position - 3350;
 
  int sLM = 20;
  int sRM = -20;

 /* static byte done = 1;
  if (done) {
    done = 0;
    roboShield.resetEncoder(ENCODER_0_PIN);
    roboShield.resetEncoder(ENCODER_1_PIN);
  } */

  if(error < 0) { // left of line, speed up left motor
    sRM -= ((double) abs(error) ) / 3350 * 80;
  }
  else if(error > 0) { // right of line, speed up right motor 
    sLM += ((double) abs(error)) / 3350 * 80;
  }
  
  /*if (error >= ON_RIGHT) // line is to the right
    sRM = 0;
  if (error >= ON_RIGHT && error <= SMOOTH_RIGHT)
    sRM = -10;
  if (error <= ON_LEFT) // line is to the left
    sLM = 0;
  if (error <= ON_LEFT && error >= SMOOTH_LEFT)
    sLM = 10; */

  if (rs.readEncoder(ENCODER_0_PIN) > distance || rs.readEncoder(ENCODER_1_PIN) > distance) {
    sLM = 0;
    sRM = 0;
  }

  Serial.print("L: ");
  Serial.println(sLM);
  Serial.print("R: ");
  Serial.println(sRM);
    
  rs.setMotor(rightMotor, sRM);
  rs.setMotor(leftMotor, sLM);
}
/*
int driveDistance(int Emotor, int distance)
{
  static byte done = 1;
  if (done) {
    done = 0;
    roboShield.resetEncoder(ENCODER_0_PIN);
    roboShield.resetEncoder(ENCODER_1_PIN);
  }
  if (Emotor == 2) {
    roboShield.setMotor(Emotor, 50);
    if (roboShield.readEncoder(ENCODER_0_PIN) > distance) {
      roboShield.setMotor(Emotor, 0);
      done = 1;
    }

  }
  else if (Emotor == 3) {
    roboShield.setMotor(Emotor, 50);
    if (roboShield.readEncoder(ENCODER_1_PIN) > distance) {
      roboShield.setMotor(Emotor, 0);
      done = 1;
    }
  }
  else
    Serial.print("Your an idiot");
  return done;
}
*/

void turnLeft() {
    rs.resetEncoder(ENCODER_0_PIN);
    rs.resetEncoder(ENCODER_1_PIN);

    unsigned int distance = 525;
    
    while(rs.readEncoder(ENCODER_1_PIN) < distance || rs.readEncoder(ENCODER_0_PIN) < distance) {
      rs.setMotor(rightMotor, -20);
      rs.setMotor(leftMotor, -20);
    }
    rs.setMotor(rightMotor, 0);
    rs.setMotor(leftMotor, 0);
}

void turnRight() {
    rs.resetEncoder(ENCODER_0_PIN);
    rs.resetEncoder(ENCODER_1_PIN);

    unsigned int distance = 525;
    
    while(rs.readEncoder(ENCODER_1_PIN) < distance || rs.readEncoder(ENCODER_0_PIN) < distance) {
      rs.setMotor(rightMotor, 20);
      rs.setMotor(leftMotor, 20);
    }
    rs.setMotor(rightMotor, 0);
    rs.setMotor(leftMotor, 0);
}


void turnAround() {
    rs.resetEncoder(ENCODER_0_PIN);
    rs.resetEncoder(ENCODER_1_PIN);

    unsigned int distance = 1215;
    
    while(rs.readEncoder(ENCODER_1_PIN) < distance || rs.readEncoder(ENCODER_0_PIN) < distance) {
      rs.setMotor(rightMotor, 20);
      rs.setMotor(leftMotor, 20);
    }
    rs.setMotor(rightMotor, 0);
    rs.setMotor(leftMotor, 0);  
}


void loop() {
  //if(rs.buttonPressed() == 1){
    rs.lcdClear();
    rs.lcdPrintf("Following Line.");
      while(1){
        /*unsigned int sensors[SENSOR_COUNT];               
        int position = qtr.readLine(sensors);
        Serial.println(position - 3350); */
        //turnRight();
        //FollowLine(0, position);
        Serial.println(rs.buttonPressed());
        delay(200);
      }
    }
  //}
//}
