/************************************************************************
 * robo_tester
 * 
 * AUTHOR: Brandon Arnold
 * CREATED: 16:30, 14 April 2016
 * DESCRIPTION: This program will be running on an Arduino Uno equipped w/
 *   an Adafruit Servo/PWM shield capable of controlling up to 16 servos or
 *   PWM channels. The program is meant to control 3 robotic arms (3 servos
 *   each) each equipped w/ 1 grabber (1 servo each) to grab or place PVC 
 *   rings with an external diameter of 2.375". 
 *   
 *   This robot will be competing as a part of Cal Poly's Roborodentia XXI
 *   which is a competition put on every year by the Roborodentia club as a
 *   part of Cal Poly's Open House, in particular as a part of the CPE/EE
 *   showcase.
 *   
 *   This particular program allows for serial communication with the Uno
 *   to define the angle of a particular servo, so that I can determine
 *   the angles needed for the different states that the robotic arms will
 *   be in, depending on the interrupts received from the Arduino MEGA that
 *   is driving the motor encoders.
 *   
 * CONSIDERATIONS: 
 *   - The servos will potentially draw a significant amount of current 
 *     when moving all at the same time. This current will be investigated
 *     at several times by using a current-monitoring power supply to power
 *     the servo shield. To be extra safe, especially before the qualifying
 *     runs, I'll probably to run one arm at a time.
 *   - It may be worth investigating whether to move the servos more slowly
 *     than their maximum speed in order to not jeopardize the physical arm
 *     and it's stability.
 * 
 * NOTES:
 *   - Some of this code is adopted from the example provided as a part of
 *     the Adafruit library associated with the Adafruit Servo/PWM shield.
 *   - The number associated with the servos will increase from the base of
 *     the robotic arm to the tip. The first 'shoulder' servos will thus be
 *     numbered 0,4,8; the second 'elbow' servos will be 1,5,9; the third
 *     'wrist' servos will be numbered 2,6,10; finally, the servos driving
 *     the grabbers will be designated 3,7,11.
 *   - The above is somewhat false as I've modified the middle arm to only
 *     be comprised of 2 servos (shoulder & wrist) rather than 4.
 *   - The servos responsible for turning the right and left arms will be
 *     numbered 12 (left) and 13 (right).
 * 
 *************************************************************************/
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMID  375 // approximate middle of the range shown above

// global servo # counter
uint8_t servonum = 0;
int degree;

// booleans
boolean gotServo = false;
boolean gotDegree = false;

// String read from serial
String readString;

void setup() {
  Serial.begin(9600);
  Serial.println("Welcome force-user to the Jedi Aca--- testing environment");
  Serial.println("After a command has been enter, the result of the command will be presented as:");
  Serial.println("\t( servonum, degree )");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  yield();
}

void loop() {
  int pulseLen;

  Serial.println("Young Jedi, enter the digit of the so-called 'servo' you seek to understand!");
  while(gotServo == false) { 
    while (Serial.available()) {
      char c = Serial.read();  //gets one byte from serial buffer
      if(c != '\r' && c != '\n'){
        readString += c; //makes the string readString
      }
      delay(2);  //slow looping to allow buffer to fill with next character
    }

    if (readString.length() >0) {
      servonum = readString.toInt();  //convert readString into a number
      //Serial.println("Input value: " + readString);
      gotServo = true;
      readString=""; //empty for next input
    } 
  }
  Serial.println("Enter the angle to seek to:");
  while(gotDegree == false) {
    while (Serial.available()) {
      char c = Serial.read();  //gets one byte from serial buffer
      if(c != '\r' && c != '\n'){
        readString += c; //makes the string readString
      }
      delay(2);  //slow looping to allow buffer to fill with next character
    }

    if (readString.length() >0) {
      degree = readString.toInt();  //convert readString into a number
      //Serial.println("Input value: " + readString);
      gotDegree = true;
      readString=""; //empty for next input
    } 
  }

  pulseLen = map(degree, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servonum, 0, pulseLen); // Set the PWM channel of servonum to the correct angle
  Serial.print("\tResult: ( ");
  Serial.print(servonum, DEC);
  Serial.print(", "); 
  Serial.print(degree, DEC);
  Serial.println(" )");

  gotServo = false;
  gotDegree = false;
}

