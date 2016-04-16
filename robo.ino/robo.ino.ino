#include <QTRSensors.h>
#include <RoboShield.h>
#include <RoboShield_Defines.h>
#include <Wire.h>
#define rightMotor 2
#define leftMotor 3
#define ON_BLACK 250
#define NUM_SENSORS 8

#define ON_RIGHT 500
#define SMOOTH_RIGHT 1500
#define ON_LEFT -500
#define SMOOTH_LEFT -1500

#define SIX_IN 505
#define THREE_IN 235

QTRSensorsRC qtr((unsigned char[]) {31, 33, 35,37,39,41, 43, 45}, 8);
RoboShield rs(0);

int ledPin = 22;                 // LED connected to digital pin 13

unsigned int calibratedMinimumOn[] = {212,264,260,260,256,260,260,312};//{212,260,212,212,212,260,264,364};
unsigned int calibratedMaximumOn[] = {4000,4000,4000,4000,4000,4000,4000,4000};//{4000,3856,3688,3076,2740,3584,4000,4000};
void setup() {
  if (rs.buttonPressed()) {
    rs.debuggingMode();
  }

  
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pinMode(ledPin, OUTPUT);      // sets the digital pin as output

  qtr.calibratedMinimumOn = calibratedMinimumOn;
  qtr.calibratedMaximumOn = calibratedMaximumOn;
}

byte FollowLine(int position, unsigned int sensors[8]) {
  
  int error = position - 3350;
 
  int sLM = 19;
  int sRM = -17;

  static byte done = 1;
  if (done) {
    done = 0;
    rs.resetEncoder(ENCODER_0_PIN);
    rs.resetEncoder(ENCODER_1_PIN);
  } 

  if(error < 0) { // left of line, speed up left motor
    sRM -= ((double) abs(error) ) / 3350 * 10;
  }
  else if(error > 0) { // right of line, speed up right motor 
    sLM += ((double) abs(error)) / 3350 * 10;
  }
 
  Serial.print("L: ");
  Serial.println(sLM);
  Serial.print("R: ");
  Serial.println(sRM);
  
  byte sensor_count = 0;
  
  for(int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("[");
    Serial.print(i);
    Serial.print("]=");
    Serial.print(sensors[i]);
    Serial.print(" ");
    if(sensors[i] > ON_BLACK)
      sensor_count++;
  }
  Serial.println();
  if(sensor_count >= 3){
    Serial.println("Hit fork");
    sRM = 0;
    sLM = 0;
    rs.setMotor(rightMotor, sRM);
    rs.setMotor(leftMotor, sLM);
    done = 1;
    return 0;
  }
  //RIGHT == MOTOR 2 == ENCODER 0
  //LEFT == MOTOR 3 == ENCODER 1
  /*if(rs.readEncoder(ENCODER_1_PIN) - rs.readEncoder(ENCODER_0_PIN) > 10) { //potential encoder correction
    sLM--;
  }*/
  
  rs.setMotor(rightMotor, sRM);
  rs.setMotor(leftMotor, sLM);

  return 1;
}

void driveDistance(int distance)
{
  while(rs.readEncoder(ENCODER_1_PIN) < distance || rs.readEncoder(ENCODER_0_PIN) < distance) {
      rs.setMotor(rightMotor, -12);
      rs.setMotor(leftMotor, 13);
    }
    rs.setMotor(rightMotor, 0);
    rs.setMotor(leftMotor, 0);

    Serial.print("You're an idiot");
}


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

    unsigned int distance = 1220;
    
    while(rs.readEncoder(ENCODER_1_PIN) < distance || rs.readEncoder(ENCODER_0_PIN) < distance) {
      rs.setMotor(rightMotor, 20);
      rs.setMotor(leftMotor, 20);
    }
    rs.setMotor(rightMotor, 0);
    rs.setMotor(leftMotor, 0);  
}


void loop() {
  static byte position = 0;
  if(rs.buttonPressed() == 1){
    rs.lcdClear();
    rs.lcdPrintf("Following Line.");
    unsigned int sensors[8];
      while(1){
        turnAround();
        delay(500);
        while(FollowLine(qtr.readLine(sensors), sensors)) {
          ;
        }

        delay(500);
        turnRight();
        delay(500);
        while(FollowLine(qtr.readLine(sensors), sensors)) {
          ;
        }
        delay(500);
        turnAround();
        delay(500);
        while(FollowLine(qtr.readLine(sensors), sensors)) {
          ;
        }
        delay(500);
        turnLeft();
        delay(500);
        while(FollowLine(qtr.readLine(sensors), sensors)) {
          ;
        }
        delay(5000);
      }
      }
    }
 // }
//}
