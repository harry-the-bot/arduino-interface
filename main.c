bool isMovingForward = false;
bool isMovingBackward = false;
bool isMovingLeft = false;
bool isMovingRight = false;

void setup() {

    pinMode(22, OUTPUT);
    pinMode(23, OUTPUT);
    pinMode(24, OUTPUT);
    pinMode(25, OUTPUT);

    Serial.begin(9600);

}

void loop() {

    String str = readSerial();

    if(str.length() > 0){

       if(str.charAt(0) == 'M'){
          setCurrentMovement(str.charAt(1),100);
       }

    }

    keepMoving();


}

String readSerial(){

    String str;

    while(Serial.available() > 0) {
       delay(1);
       char c = Serial.read();
       if(c == '\n')
          break;
       str += c;
    }

    if(str.length() > 0){

       return str;

    }

    return "";

}

void keepMoving(){
    if(isMovingForward){
       showNumber(1);
    }

    if(isMovingBackward){
       showNumber(2);
    }

    if(isMovingLeft){
       showNumber(3);
    }

    if(isMovingRight){
       showNumber(4);
    }

    if( !isMovingForward &&
          !isMovingBackward &&
          !isMovingLeft &&
          !isMovingRight )
       showNumber(7);
}

void setCurrentMovement(char direction, int speed){

    isMovingForward = false;
    isMovingBackward = false;
    isMovingLeft = false;
    isMovingRight = false;

    switch(direction){
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

void turnAllOff(){
    digitalWrite(22, LOW);
    digitalWrite(23, LOW);
    digitalWrite(24, LOW);
    digitalWrite(25, LOW);
}

void showNumber(int number){
       turnAllOff();
       switch(number){
           case 0:
             turnAllOff();
             break;
           case 1:
             digitalWrite(22, HIGH);
             break;
           case 2:
             digitalWrite(23, HIGH);
             break;
           case 3:
             digitalWrite(22, HIGH);
             digitalWrite(23, HIGH);
             break;
           case 4:
             digitalWrite(24, HIGH);
             break;
             break;
           case 5:
             digitalWrite(24, HIGH);
             digitalWrite(22, HIGH);
             break;
           case 6:
             digitalWrite(24, HIGH);
             digitalWrite(23, HIGH);
             break;
           default:
             digitalWrite(22, HIGH);
             digitalWrite(23, HIGH);
             digitalWrite(24, HIGH);

        }

}
