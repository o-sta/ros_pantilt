#define USE_USBCON
#define PAN_MAX 180           //panの最大角度[deg]
#define PAN_MIN 0             //panの最小角度[deg]
#define TILT_MAX 140          //tiltの最大角度[deg]
#define TILT_MIN 0            //tiltの最小角度[deg]
#define PAN_VELOCITY_MAX 255  //pan速度の最大値[単位無し]
#define PAN_VELOCITY_MIN 0    //pan速度の最小値[単位無し]
#define TILT_VELOCITY_MAX 255 //tilt速度の最小値[単位無し]
#define TILT_VELOCITY_MIN 0   //tilt速度の最小値[単位無し]
#define PAN_INIT 90           //panの初期値
#define TILT_INIT 104         //tiltの初期値
#define PAN_VELOCITY_INIT 0   //pan速度の初期値（0でfull 1-255で低速-高速）
#define TILT_VELOCITY_INIT 0   //tilt速度の初期値（0でfull 1-255で低速-高速）
#include <ros.h>
#include <std_msgs/UInt16MultiArray.h>
#include <VarSpeedServo.h>

ros::NodeHandle_<ArduinoHardware, 3, 3, 128, 256> nh;
//ros::NodeHandle nh;
VarSpeedServo pan_servo;
VarSpeedServo tilt_servo;
uint16_t pan = PAN_INIT;
uint16_t tilt = TILT_INIT;
uint16_t pan_velocity = PAN_VELOCITY_INIT;
uint16_t tilt_velocity = TILT_VELOCITY_INIT;

void messageCb( const std_msgs::UInt16MultiArray& msg) {
  if(msg.data[0] >= PAN_MIN && msg.data[0] <= PAN_MAX){
    pan = msg.data[0];
  }
  if(msg.data[1] >= TILT_MIN && msg.data[1] <= TILT_MAX){
    tilt = msg.data[1];
  }
  if(msg.data[2] >= PAN_VELOCITY_MIN && msg.data[2] <= PAN_VELOCITY_MAX){
    pan_velocity = msg.data[2];
  }
  if(msg.data[3] >= TILT_VELOCITY_MIN && msg.data[3] <= TILT_VELOCITY_MAX){
    tilt_velocity = msg.data[3];
  }
}

ros::Subscriber<std_msgs::UInt16MultiArray> sub("PanTilt", &messageCb );

void setup() {
  pan_servo.attach(11);
  tilt_servo.attach(5);
  nh.getHardware()->setBaud(115200);
  nh.initNode();  
  nh.subscribe(sub);
}

void loop() {
  pan_servo.write(pan, pan_velocity, false);
  tilt_servo.write(tilt, tilt_velocity, false);
  nh.spinOnce();
}
