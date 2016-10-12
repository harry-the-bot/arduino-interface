#include <AFMotor.h>
#include "HC_SR04.h"

//SENSORS
int frontLeftTrigger = 22;
int frontRightTrigger = 24;
int leftRightTrigger = 26;
int leftLeftTrigger = 28;
int rightRightTrigger = 30;
int rightLeftTrigger = 32;
int backRightTrigger = 34;
int backLeftTrigger = 36;

int frontLeftEcho = 23;
int frontRightEcho = 25;
int leftRightEcho = 27;
int leftLeftEcho = 29;
int rightRightEcho = 31;
int rightLeftEcho = 33;
int backRightEcho = 35;
int backLeftEcho = 37;

// ENGINES
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// SENSORS
HC_SR04 sensorFrontLeft(frontLeftTrigger, frontLeftEcho, 0);
HC_SR04 sensorFrontRight(frontRightTrigger, frontRightEcho, 1);
HC_SR04 sensorRightRight(rightRightTrigger, rightRightEcho, 2);
HC_SR04 sensorRightLeft(rightLeftTrigger, rightLeftEcho, 3);
HC_SR04 sensorLeftRight(leftRightTrigger, leftRightEcho, 4);
HC_SR04 sensorLeftLeft(leftLeftTrigger, leftLeftEcho, 5);
HC_SR04 sensorBackRight(backRightTrigger, backRightEcho, 6);
HC_SR04 sensorBackLeft(backLeftTrigger, backLeftEcho, 7);

// Globals
bool isMovingForward = false;
bool isMovingBackward = false;
bool isMovingLeft = false;
bool isMovingRight = false;
bool saidHello = false;

long frontLeftDistance = 0;
long frontRightDistance = 0;
long leftLeftDistance = 0;
long leftRightDistance = 0;
long backLeftDistance = 0;
long backRightDistance = 0;
long rightLeftDistance = 0;
long rightRightDistance = 0;
bool startedSensors = false;

void setup() {

  Serial.begin(9600);
    
  //ENGINES
  motor1.setSpeed(255);
  motor1.run(RELEASE);

  motor2.setSpeed(255);
  motor2.run(RELEASE);

  motor3.setSpeed(255);
  motor3.run(RELEASE);

  motor4.setSpeed(255);
  motor4.run(RELEASE);

  
}

void loop() {

  if (!saidHello) {
    Serial.write("HEL");
    saidHello = true;
  }

  String str = readSerial();

  if (str.length() > 1) {
    Serial.print(str);
    if (str.charAt(0) == 'M') {
      setCurrentMovement(str.charAt(1), 100);
    }

  }

  keepMoving();
  /*Serial.print(frontLeftDistance);
  Serial.print(" - ");
  Serial.print(frontRightDistance);
  Serial.print("\r\n");*/
  if(!startedSensors){
    sensorFrontLeft.start();
    sensorFrontRight.start();
    sensorLeftLeft.start();
    sensorLeftRight.start();
    sensorRightLeft.start();
    sensorRightRight.start();
    sensorBackLeft.start();
    sensorBackRight.start(); 
    startedSensors = true;
  }
  checkSensors();

}


void checkSensors() {
  
  if(sensorFrontLeft.isFinished()){
      frontLeftDistance = sensorFrontLeft.getRange();
      sensorFrontLeft.start();
  }
  if(sensorFrontRight.isFinished()){
      frontRightDistance = sensorFrontRight.getRange();
      sensorFrontRight.start();
  }
  if(sensorRightRight.isFinished()){
      rightRightDistance = sensorRightRight.getRange();
      sensorRightRight.start();
  }
  if(sensorRightLeft.isFinished()){
      rightLeftDistance = sensorRightLeft.getRange();
      sensorRightRight.start();
  }
  if(sensorLeftRight.isFinished()){
      leftRightDistance = sensorLeftRight.getRange();
      sensorLeftRight.start();
  }
  if(sensorLeftLeft.isFinished()){
      leftLeftDistance = sensorLeftLeft.getRange();
      sensorLeftRight.start();
  }
  if(sensorBackRight.isFinished()){
      backRightDistance = sensorBackRight.getRange();
      sensorBackRight.start();
  }
  if(sensorBackLeft.isFinished()){
      backLeftDistance = sensorBackLeft.getRange();
      sensorBackRight.start();
  }
}



/**************************************************
 *
 *					ENGINE
 *
 *************************************************/
String readSerial() {

  String str = "";

  while (Serial.available() > 0) {
    delay(1);
    char c = (char) Serial.read();

    if (c == ';')
    break;

    str += c;

  }

  if (str.length() > 0) {

    return str;

  }

  return "";

}

void keepMoving() {
  if (isMovingForward) {
    goForward();
  }

  if (isMovingBackward) {
    goBackward();
  }

  if (isMovingLeft) {
    turnLeft();
  }

  if (isMovingRight) {
    turnRight();
  }

  if (!isMovingForward &&
    !isMovingBackward &&
    !isMovingLeft &&
    !isMovingRight)
    stayStill();
}

void setCurrentMovement(char direction, int speed) {

  isMovingForward = false;
  isMovingBackward = false;
  isMovingLeft = false;
  isMovingRight = false;

  switch (direction) {
  case 'F':
    isMovingForward = true;
    break;
  case 'B':
    isMovingBackward = true;
    break;
  case 'L':
    isMovingLeft = true;
    break;
  case 'R':
    isMovingRight = true;
    break;
  default:
    isMovingForward = false;
    isMovingBackward = false;
    isMovingLeft = false;
    isMovingRight = false;

  }

}

void stayStill(){
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);
    delay(300);
}

void goForward(){
    motor1.run(FORWARD);
    motor2.run(FORWARD);
}

void goBackward(){
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
}

void turnLeft(){
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
}

void turnRight(){
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
}
