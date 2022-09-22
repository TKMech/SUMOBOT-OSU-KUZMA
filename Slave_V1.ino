// Slave_V1
// Created by: Tristan Kuzma
// 05/08/22
// MIME498 - MIME CAPSTONE DESIGN
// TEAM 605.1 Sumobot

#include "CytronMotorDriver.h"
#include <Wire.h>

 // Configure the motor driver.
 //Adafruit Metro = Slave 
 //PMW Pins = 3, 5, 6, 9, 10, 11
  //Motor Driver 1//
  CytronMD motor1(PWM_DIR, 3, 4);  // PWM 1 = Pin 3, DIR 1 = Pin 4.
  CytronMD motor2(PWM_DIR, 5, 6); // PWM 2 = Pin 5, DIR 2 = Pin 6.

  //Motor Driver 2//
  CytronMD motor3(PWM_DIR, 9, 10); // PWM 1 = Pin 9, DIR 1 = Pin 10.
  CytronMD motor4(PWM_DIR, 11, 12); // PWM 2 = Pin 11, DIR 2 = Pin 12.


  //Integers for Arduino Communication.
int x;
float t = 0; 

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed

Serial.println("Slave Initializing..."); // print some text in Serial Monitor
 
 // Start the I2C Bus as Slave on address 9
  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
  delay(10);
  x = Wire.read();    // read one character from the I2C
t = millis()/1000; 
} 

void loop() {
  // put your main code here, to run repeatedly:
Serial.print("Read, x = ");
  Serial.println(x);
  Serial.print("Time = "); 
  Serial.println(t);

  if (x == 1)
  {
   Serial.println("90 Deg Left");
   // void SnapL(); 
  }

  else if(x == 2)
  {
   Serial.println("Slight Left");
  // void SL(); 
  }
  else if(x == 3)
  {
   Serial.println("Go Straight");
 //  void FORWARD();
  }
   else if(x == 4)
  {
   Serial.println("Slight Right");
  // void SR(); 
  }
   else if(x == 5)
  {
   Serial.println("90 Deg Right");
 //  void SnapR(); 
  }
  else if(x == 6)
  {
    Serial.println("Seek");
   // void SEEK(); 
  }
   else if(x == 7)
  {
    Serial.println("STOP");
   // void STOP(); 
  }

  delay(2000);
  }

///BASIC MOTOR CONTROL LOOPS///

//Motor 1 = Left Front 
//Motor 2 = Right Front 
//Motor 3 = Left Rear
//Motor 4 = Right Rear

int Out = 64;  //This value ranges from 0 - 255. DONT RUN AT MAX WHILE TESTING
int turnMod = 0.8; //Used to create curved turning. 
int scale = 1000; //This value will range pending the Output 
int BackupTime = 0; //This delay is used to determine how long to back up. 


//If Out = 64, scale = 
//If Out = 128, scale = 
//If Out = 192, scale = 
//If Out = 255, scale = 

  void SEEK(){
  //How are we starting? Face to face? Side to side?
  //IF Side - Side, What side will be facing? Right or lef?
  //I DONT want to give my opponent my back, I have NO Rear US sensors. 
  //EMAIL OMAN
}
  void SnapL(){
    //Snap 90 Deg Lef
motor1.setSpeed(-Out);
motor2.setSpeed(Out);
motor3.setSpeed(-Out);
motor4.setSpeed(Out);
    
    delay(scale);
    
STOP(); 
return; 

  }

  void SL(){

motor1.setSpeed(Out*turnMod);
motor2.setSpeed(Out);
motor3.setSpeed(Out*turnMod);
motor4.setSpeed(Out);  

  delay(scale);
    
STOP(); 
return; 
    
  }

  void FORWARD(){
  
motor1.setSpeed(Out);
motor2.setSpeed(Out);
motor3.setSpeed(Out);
motor4.setSpeed(Out);  

  delay(scale);
    
STOP(); 
return;

  }

  void SR(){

motor1.setSpeed(Out);
motor2.setSpeed(Out*turnMod);
motor3.setSpeed(Out);
motor4.setSpeed(Out*turnMod);  

  delay(scale);
    
STOP(); 
return;    
  }

  void SnapR(){
  //Snap 90 Deg Right
motor1.setSpeed(Out);
motor2.setSpeed(-Out);
motor3.setSpeed(Out);
motor4.setSpeed(-Out);

delay(scale);
    
STOP(); 
return;

    
  }

  void REVERSE(){
motor1.setSpeed(-Out);
motor2.setSpeed(-Out);
motor3.setSpeed(-Out);
motor4.setSpeed(-Out);

delay(BackupTime);

STOP(); 
return; 


  
    
  }

  void STOP(){ 
motor1.setSpeed(0);
motor2.setSpeed(0);
motor3.setSpeed(0);
motor4.setSpeed(0);
return;
  }

  
  
