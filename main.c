#include <AFMotor.h>

//LEDS
int redLed = 44;
int greenLed = 45;
int blueLed = 46;

int redLedValue = 255;
int greenLedValue = 255;
int blueLedValue = 255;
int ledState = 1; //0 = static, 1 = blink;
int ledLastBlink = 0;
int ledLastBlinkState = 0;//0 = off, 1 = on

//SENSORS
long frontLeftDistance = 0;
long frontRightDistance = 0;
long leftLeftDistance = 0;
long leftRightDistance = 0;
long backLeftDistance = 0;
long backRightDistance = 0;
long rightLeftDistance = 0;
long rightRightDistance = 0;

// ENGINES
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// Globals
bool isMovingForward = false;
bool isMovingBackward = false;
bool isMovingLeft = false;
bool isMovingRight = false;
bool saidHello = false;
unsigned long lastSensorReport = 0;;

void setup() {

  Serial.begin(9600);
  Serial2.begin(9600);
  
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  
  //ENGINES
  /*motor1.setSpeed(255);
  motor1.run(RELEASE);*/

  motor2.setSpeed(200);
  motor2.run(RELEASE);

  motor3.setSpeed(200);
  motor3.run(RELEASE);

  /*motor4.setSpeed(255);
  motor4.run(RELEASE);*/

}


void loop() {
    
  if (!saidHello) {
    Serial.write("HELLO;");
    saidHello = true;
  }

  checkLeds();
  checkEngines();
  updateSensors();

  refreshLedColor(); 
  //reportSensors();   
  keepMoving();
  
}

/**************************************************
 *
 *          LEDS
 *
 *************************************************/
 
void checkLeds(){
  String str = readSerial0();

  if (str.length() < 1) 
    return;
    
  if (str.charAt(0) != 'L') {
    return;
  }

  //LC000000
  if(str.charAt(1) == 'C') {
    String hexColor = "#";
    hexColor += str.substring(2,8);
    setLedColor(hexColor);
  }

  //LS1
  if(str.charAt(1) == 'S'){
    if(str.charAt(2) == '0'){
      ledState = 0;
    }

    if(str.charAt(2) == '1'){
      ledState = 1;
    }
  }

}

void setLedColor(String hexColor){
  long number = strtol( &hexColor[1], NULL, 16);
  
  redLedValue = number >> 16;
  greenLedValue = number >> 8 & 0xFF;
  blueLedValue = number & 0xFF;  

}

void refreshLedColor(){
  if(ledState == 0){
    turnLedsOn();
  }else{
    unsigned long now = millis();
    if(now - ledLastBlink > 500){
      if(ledLastBlinkState == 1){
        turnLedsOff();
        ledLastBlinkState = 0;
      }else{
        turnLedsOn();
        ledLastBlinkState = 1;
      }
      ledLastBlink = millis();
    }
  }
}

void turnLedsOn(){
  analogWrite(redLed, redLedValue);
  analogWrite(greenLed, greenLedValue);
  analogWrite(blueLed, blueLedValue);
}

void turnLedsOff(){
  analogWrite(redLed, 0);
  analogWrite(greenLed, 0);
  analogWrite(blueLed, 0);
}
/**************************************************
 *
 *          SENSORS
 *
 *************************************************/

void reportSensors(){
  unsigned long now = millis();
  if(now - lastSensorReport < 700)
    return;
  reportSensor('F','L',frontLeftDistance);
  reportSensor('F','R',frontRightDistance);
  reportSensor('L','L',leftLeftDistance);
  reportSensor('L','R',leftRightDistance);
  reportSensor('B','L',backLeftDistance);
  reportSensor('B','R',backRightDistance);
  reportSensor('R','L',rightLeftDistance);
  reportSensor('R','R',rightRightDistance);
}

void reportSensor(char location, char place, int distance){
  String report = "S";
  report += location;
  report += place;
  report += distance;
  report += ";\r\n";

  Serial.print(report);
}

//@TODO Refactor this, please.
void updateSensors(){
  
  String s = readSerial2();
  if(s.charAt(0) == 'S'){
    
    String strDistance = s.substring(3,6);
    int distance = strDistance.toInt();

    //FRONT SENSORS
    if(s.charAt(1) == 'F'){
      
      if(s.charAt(2) == 'L'){
        frontLeftDistance = distance;          
      }
      if(s.charAt(2) == 'R'){
        frontRightDistance = distance;          
      }
    }//FRONT SENSORS

    
    //LEFT SENSORS
    if(s.charAt(1) == 'L'){
      
      if(s.charAt(2) == 'L'){
        leftLeftDistance = distance;          
      }
      if(s.charAt(2) == 'R'){
        leftRightDistance = distance;          
      }
    }//LEFT SENSORS

    //BACK SENSORS
    if(s.charAt(1) == 'B'){
      
      if(s.charAt(2) == 'L'){
        backLeftDistance = distance;          
      }
      if(s.charAt(2) == 'R'){
        backRightDistance = distance;          
      }
    }//BACK SENSORS

    //RIGHT SENSORS
    if(s.charAt(1) == 'R'){
      
      if(s.charAt(2) == 'L'){
        rightLeftDistance = distance;          
      }
      if(s.charAt(2) == 'R'){
        rightRightDistance = distance;          
      }
    }//RIGHT SENSORS
    
  }//is sensor report
  
}
String readSerial2() {

  String str = "";
  while (Serial2.available() > 0) {
    delay(1);
    char c = (char) Serial2.read();

    if (c == ';')
      break;

    str += c;

  }

  if (str.length() > 0) {

    return str;

  }

  return "";

}


/**************************************************
 *
 *          ENGINE
 *
 *************************************************/

void checkEngines(){
  String str = readSerial0();

  if (str.length() > 1) {
    Serial.print(str);
    if (str.charAt(0) == 'M') {
      setCurrentMovement(str.charAt(1), 100);
    }

  }
}
String readSerial0() {

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
    //motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    //motor4.run(RELEASE);
    delay(300);
}

void goForward(){
    motor2.run(FORWARD);
    motor3.run(FORWARD);
}

void goBackward(){
    
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
}

void turnLeft(){
    motor2.run(FORWARD);
    motor3.run(BACKWARD);
}

void turnRight(){
    motor2.run(BACKWARD);
    motor3.run(FORWARD);
}
