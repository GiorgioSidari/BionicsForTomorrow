#include "Wire.h"       
#include "I2Cdev.h"     
#include "MPU6050.h"    
#include <L298NX2.h>
#include "math.h" 
#define SENSITIVITY 0.1
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;



// Pin definition
const unsigned int in1 = 6;
const unsigned int in2 = 7;
const unsigned int in3 = 8;
const unsigned int in4 = 9;
const unsigned int enA = 10;
const unsigned int enB = 5;
double old_value = 0;
int counter =0;
bool mouthIsOpen = false;



// Create one motor instance
L298NX2 myMotors(enA, in1, in2, enB , in3 , in4);



struct MyData {
  byte X;
  byte Y;
  byte Z;
};

MyData data;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  myMotors.setSpeed(50);
  myMotors.backwardA();
  myMotors.backwardB();

}

void loop()
{
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  data.X = map(ax, -16384, 16384, 0, 255);
  data.Y = map(ay, -17000, 17000, 0, 255);
  data.Z = map(az, -17000, 17000, 0, 255); 
  double accelerationX = (data.X * 9.81) / 255;
  double accelerationY = (data.Y * 9.81) / 255;
  double accelerationZ = (data.Z * 9.81) / 255;
  double acceleration = sqrt(accelerationX*accelerationX + accelerationY*accelerationY + accelerationZ*accelerationZ);
  delay(1000);
 
  Serial.println(acceleration);

    if(acceleration<=old_value +SENSITIVITY && acceleration >= old_value - SENSITIVITY){
      if(counter>3 && counter<6){
        myMotors.forwardA();
        myMotors.forwardB();
        delay(550);
        myMotors.stop();

        mouthIsOpen = true;
        delay(500);
      }   
        counter++;
    }else{
      if(mouthIsOpen){

        myMotors.backwardA();
        myMotors.backwardB();
        mouthIsOpen = false;
      }
      counter =1;
      old_value = acceleration;

    }





}
