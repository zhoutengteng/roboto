/* Motor Board IR Array Test

   This example of the Arduno robot's motor board returns the
   values read fron the 5 infrared sendors on the bottom of
   the robot.

*/
// include the motor board header

#include <Servo.h>


String bar; // string for storing the informaton
// 按顺序接上　２,3,4,5 接　in1 in2 in3 in4
int LEFTHIGH = 2;
int LEFTLOW = 3;
int RIGHTLOW = 4; 
int RIGHTHIGH = 5;

int LEFTSENSOR = 40;
int MIDDLESENSOR = 44;
int RIGHTSENSOR = 22;

int URPWM = 48; // PWM Output 0－25000US，Every 50US represent 1cm
int URTRIG=50; // PWM trigger pin

unsigned int distance=0;
uint8_t EnPwmCmd[4]={0x44,0x02,0xbb,0x01};    // distance measure command

Servo myservo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

int LARGE = 1000;
int SMALL = 1000;
int minDistance = LARGE;
int minDistanceIndex = SMALL;
int MOTIVATION = 150;
int lastpos = 0;
int eye = 20;

//判断是否需要转动摄像头
boolean isTurn = true;
boolean isTooNear = false;
boolean isTooFar = false;
boolean isChanged = false;

void setup() {
     Serial.begin(9600);
 
}


//应该将测距集成到驼机转动中

void loop() {

    readSensor123();
   
}




void readSensor123() {
    int left = digitalRead(LEFTSENSOR);
    int mid = digitalRead(MIDDLESENSOR);
    int right = digitalRead(RIGHTSENSOR);
    Serial.print(" left :");
    Serial.print(digitalRead(LEFTSENSOR));
    Serial.print(" middle ");
    Serial.print(digitalRead(MIDDLESENSOR));
    Serial.print(" right :");
    Serial.println(digitalRead(RIGHTSENSOR));
    if (left == 0&& mid == 0&& right == 0) {
        runLeftByAngle(70);
    } else if (left == 0&& mid == 0&& right == 1) {
        runLeftByAngle(30);
    } else if (left == 0&& mid == 1&& right == 1) {
        runLeftByAngle(20);
    } else if (left == 1&& mid == 0&& right == 0) {
        runRightByAngle(20);
    } else if (left == 1&& mid == 0&& right == 1) {
        run_abnormal();
    } else if (left == 1&& mid == 1&& right ==  0) {
        runRightByAngle(30);
    } else {
        run_abnormal();
    }
  
}

void stopMoving() {
      analogWrite(RIGHTHIGH, 0);
      analogWrite(RIGHTLOW, 0);
      analogWrite(LEFTHIGH, 0);
      analogWrite(LEFTLOW, 0);
}

void run_abnormal_left() {
      analogWrite(LEFTHIGH,MOTIVATION);
      analogWrite(LEFTLOW,0);
}

// 又轮需要加大马力
void run_abnormal_right() {
    analogWrite(RIGHTHIGH,MOTIVATION + 10);
    analogWrite(RIGHTLOW,0);
  
}

void run_abnormal() {
   run_abnormal_left();
   run_abnormal_right();
}


void turnLeft() {
      analogWrite(RIGHTHIGH, MOTIVATION);
      analogWrite(RIGHTLOW, 0);
      analogWrite(LEFTHIGH, 0);
      analogWrite(LEFTLOW,0);
}


void turnRight() {
      analogWrite(RIGHTHIGH, 0);
      analogWrite(RIGHTLOW, 0);
      analogWrite(LEFTHIGH, MOTIVATION);
      analogWrite(LEFTLOW,0);
      
}


// x = 180 => 约大于90度;
void runLeftByAngle(int x) {
   turnLeft();
   delay(x * 5);
   run_abnormal();
}



void runRightByAngle(int x) {
   turnRight();
   delay(x * 5);
   run_abnormal();

  
}



