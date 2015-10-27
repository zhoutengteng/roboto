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
     PWM_Mode_Setup();
     myservo.attach(9);  // attaches the servo on pin 9 to the servo ob
     //myservo.attach(9);  // attaches the servo on pin 9 to the servo object
     // start serial communication
     Serial.begin(9600);
     // initialize the library
     pinMode(LEFTHIGH,INPUT);
     pinMode(LEFTLOW,INPUT);
     pinMode(RIGHTHIGH,INPUT);
     pinMode(RIGHTLOW,INPUT);
     pinMode(LEFTSENSOR, INPUT);
     pinMode(RIGHTSENSOR, INPUT);
     myservo.write(0);
     delay(2000); 
}


//应该将测距集成到驼机转动中

void loop() {

    tuoji();

   
}



void tuoji() {
  if (isTurn) {
    //当失去目标的时候要触发下
    //应该用45角行车 
    // 停止小车的运动， 用眼睛观察 to-do
    stopMoving();
    // 这里是静止时的情况
    minDistance = 1000;
    minDistanceIndex = SMALL;
    //判断行进中 是否有调整的布尔值
    myservo.write(0);
    delay(1000);
    int pos = 0;
    lastpos = myservo.read();
    for(pos = 0; pos <= 180; pos += 5) {
          PWM_Mode();
          if (distance < minDistance) {

            minDistance = distance;
            minDistanceIndex = pos;
          //  break;
          }
          myservo.write(pos);              
          delay(50);                      
      } 
      for(pos = 180; pos>=0; pos-=5) {            
          PWM_Mode();
          if (distance < minDistance) {           
            minDistance = distance;
            minDistanceIndex = pos; 
           // break;
          }
          myservo.write(pos);               
          delay(50);                     
      }
      
     // if (minDistance != LARGE && minDistanceIndex != SMALL) {
        if (minDistance <= eye) {
          isTurn = false;
          delay(300);  
          Serial.println(minDistanceIndex);
          myservo.write(minDistanceIndex);
          delay(200);
        } else {
          isTurn = true;
        }
        /*
        if (minDistanceIndex / 90 == 1) {
          myservo.write(135);
        } else {
          myservo.write(45);
        }
        */




    //  }
      
  } else {

          int ang = myservo.read();
          if (ang > 110) {
                turnLeftByAngle(50);
                isChanged = false;
                isTurn = true;
          }
          if (ang < 70){
                turnRightByAngle(50);
                isChanged = false;
                isTurn = true;
          }
          if (ang >= 70 && ang <= 110){
                isChanged = true;
                run_abnormal();
                isTurn = false;
                delay(100);
          }

      if (isChanged) {
         isTurn = false;
         PWM_Mode();
         Serial.println("here:");
         //当目标消失的时候要重新找到 后面会有检测是否需要重新定位眼睛
   
              if (distance > eye) {
           //   stopMoving();
                isTooFar = true;
               } else {
              // run_abnormal();
               isTooFar = false;
               }

         if (isTooFar == true) {
             //通过微调转向，判断是否需要用眼睛 重新选择小车方向，通过转小车，必须要找到
            //change2();
          
              int ang = myservo.read();
              Serial.print("ang = ");
               Serial.print(ang);
              Serial.print("    lastpos");
              Serial.println(lastpos);

              //  if (isChanged) {
              PWM_Mode();
              if (distance > eye) {
                 stopMoving();
                 isTooNear = false;
                 isTooFar = true;
                 isTurn = true;
              }  else {
                 isTooFar = false;
                 run_abnormal();
              }
             //  }
          }
      }
  }
}


// 先写一个未完成的change函数  主要是转变角度， 车子转一些角度，更据 眼睛的方位，来判断是往哪边转
void change1() {
    //摄像头左转20度，再测, 如果能行小转一下
    int originalAngle = myservo.read();
    boolean isCircle = true;
    int ang = 45;
    myservo.write(originalAngle);
    //更具眼睛是有优先选择的
    if (originalAngle < 100) {
             Serial.println("left");
              //往左边稍微转一点
              turnLeftByAngle(60/2);
           //   delay(50);
           //   run_abnormal();
            //  delay(100);
     } else {
              //往右边稍微转一点
              Serial.println("right");
              turnRightByAngle(60/2);
             // delay(50);
             // run_abnormal();
             // delay(100);

    }
}



void change2() {
    int originalAngle = myservo.read();
    //更具眼睛是有优先选择的
    if (originalAngle < 100) {
              Serial.println("right");
              turnRightByAngle(120/2);
          //    delay(50);
          //    run_abnormal();
           //   delay(100);
    } else {
              Serial.println("left");
              //往左边稍微转一点  由于右轮块很多，所以
              turnLeftByAngle(60/2);
             // delay(50);
             // run_abnormal();
            //  delay(100);
    }
    
}



void readSensor123() {
    int left = digitalRead(LEFTSENSOR);
    int right = digitalRead(RIGHTSENSOR);
    //Serial.print("left :");
    //Serial.print(digitalRead(LEFTSENSOR));
    //Serial.print(" right :");
    //Serial.println(digitalRead(RIGHTSENSOR));
    if (left == 0) turnRightByAngle(20);
    if (right == 0) turnLeftByAngle(20);
  
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
  /*
      analogWrite(RIGHTHIGH, MOTIVATION);
      analogWrite(RIGHTLOW, 0);
      analogWrite(LEFTHIGH, 0);
      analogWrite(LEFTLOW, MOTIVATION);
   */
      analogWrite(RIGHTHIGH, MOTIVATION);
      analogWrite(RIGHTLOW, 0);
      analogWrite(LEFTHIGH, 0);
      analogWrite(LEFTLOW,0);
}


void turnRight() {
  /*
      analogWrite(RIGHTHIGH, 0);
      analogWrite(RIGHTLOW, MOTIVATION);
      analogWrite(LEFTHIGH, MOTIVATION-10);
      analogWrite(LEFTLOW, 0);
    */
      analogWrite(RIGHTHIGH, 0);
      analogWrite(RIGHTLOW, 0);
      analogWrite(LEFTHIGH, MOTIVATION);
      analogWrite(LEFTLOW,0);
      
}


// x = 180 => 约大于90度;
void turnLeftByAngle(int x) {
   turnLeft();
   delay(x * 5);
   run_abnormal();
}



void turnRightByAngle(int x) {
   turnRight();
   delay(x * 5);
   run_abnormal();

  
}


void PWM_Mode_Setup()
{ 
  pinMode(URTRIG,OUTPUT);                     // A low pull on pin COMP/TRIG
  digitalWrite(URTRIG,HIGH);                  // Set to HIGH
  
  pinMode(URPWM, INPUT);                      // Sending Enable PWM mode command
  
  for(int i=0;i<4;i++)
  {
      Serial.write(EnPwmCmd[i]);
  } 
}

//不能多次取平均
void PWM_Mode()
{              
   // int i = 3;
  //  while (i--) {
    // a low pull on pin COMP/TRIG  triggering a sensor reading
    digitalWrite(URTRIG, LOW);
    digitalWrite(URTRIG, HIGH);               // reading Pin PWM will output pulses
     
    unsigned long DistanceMeasured=pulseIn(URPWM,LOW);
     
  //  if(DistanceMeasured>=10200)
  //  {              // the reading is invalid.
   //   Serial.println("Invalid");    
  //  }
  //  else
 //   {
      distance = DistanceMeasured/50;           // every 50us low level stands for 1cm
      Serial.print("Distance=");
      Serial.print(distance);
      Serial.println("cm");
   // }
   // distance = distance / 3;
//
}
 

