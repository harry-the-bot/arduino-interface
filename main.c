#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);


bool isMovingForward = false;
bool isMovingBackward = false;
bool isMovingLeft = false;
bool isMovingRight = false;
bool saidHello = false;

void setup() {

  Serial.begin(9600);

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
    Serial.print("HEL");
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

}

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
