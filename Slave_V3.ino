// Slave_V1
// Created by: Tristan Kuzma
// 05/08/22
//V2 Created 05/26/22
// MIME498 - MIME CAPSTONE DESIGN
// TEAM 605.1 Sumobot

#include "CytronMotorDriver.h"
#include <Wire.h>

 // Configure the motor driver.
 //Adafruit Metro = Slave 
 //PMW Pins = 3, 5, 6, 9, 10, 11
  //Motor Driver 1//
  CytronMD motor1(PWM_DIR, 11, 10);  // PWM 1 = Pin 11, DIR 1 = Pin 10.
  CytronMD motor2(PWM_DIR, 5, 4); // PWM 2 = Pin 5, DIR 2 = Pin 4.

  //Motor Driver 2//
  CytronMD motor3(PWM_DIR, 6, 7); // PWM 1 = Pin 6, DIR 1 = Pin 7.
  CytronMD motor4(PWM_DIR, 9, 8); // PWM 2 = Pin 9, DIR 2 = Pin 8.

///BASIC MOTOR CONFIG///

//Motor 1 = Front Right
//Motor 2 = Front Left

//Motor 3 = Rear Left
//Motor 4 = Rear Right 

//IR Sensor Constants//
//Front Right = IR1
//Front Left = IR2
//Rear Left = IR3 
//Rear Right = IR4

//IR Sensors 
float irSen1;
float irSen2;
float irSen3;
float irSen4;

//Scrateched Rear IR

#define irPin1 A0 
#define irPin2 A1
//#define irPin3 A2 
//#define irPin4 A3

#define irPinLED1 12
#define irPinLED2 13
//#define irPinLED3 12
//#define irPinLED4 13

volatile bool state_interrupt = false;  //Used in Interrupt
volatile long time_interrupt = 0; //Used in Debounce

int y = 0; 
int On = 0; 
int U = 0;


int powerPIN = 3; 
int standardDelay = 250; 

///BASIC MOTOR CONTROL LOOPS///

//Motor 1 = Left Front 
//Motor 2 = Right Front 
//Motor 3 = Left Rear
//Motor 4 = Right Rear

int Out = 128;  //This value ranges from 0 - 255. DONT RUN AT MAX WHILE TESTING
float turnMod = 0.8;
int scale = 325; //This value will range pending the Output 
//If Out = 64, scale = 0
//If Out = 128, scale = 325
//If Out = 192, scale = 0
//If Out = 255, scale = 0+

float A = 1.05; //Reverse Correction Factor
//  Left Side Running Faster


  //Integers for Arduino Communication.
int x;
float t = 0; 

void setup() 
{
  // put your setup code here, to run once:
Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed

//Serial.println("Slave Initializing..."); // print some text in Serial Monitor

//Interrupt for On/Off
 attachInterrupt(digitalPinToInterrupt(powerPIN), On_Off, RISING);
 

 //More IR Sensor Code
  pinMode(irPin1, INPUT);
    pinMode(irPin2, INPUT);
   //  pinMode(irPin3, INPUT);
   // pinMode(irPin4, INPUT);
    
     pinMode(irPinLED1, OUTPUT);
      pinMode(irPinLED2, OUTPUT);
  //     pinMode(irPinLED3, OUTPUT);
   //   pinMode(irPinLED4, OUTPUT);

      digitalWrite(irPinLED1, HIGH); 
        digitalWrite(irPinLED2, HIGH); 
    //      digitalWrite(irPinLED3, HIGH); 
      //      digitalWrite(irPinLED4, HIGH); 

      pinMode(powerPIN, INPUT); 
      
 // Start the I2C Bus as Slave on address 9
  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
}


void On_Off() 
{
  if (!state_interrupt) {
    //state_interrupt = true;

    if (millis() - time_interrupt > standardDelay)
    {
      if (On == 0) {
        On = 1;
        time_interrupt = millis();
      
       
      }

      else if (On == 1) {
        On = 0;
        STOP(); 
        time_interrupt = millis();
       
          //Serial.println("Turning System Off...");
  
}
    }
  }
}

void receiveEvent(int bytes)
{
  

  delay(10);
  x = Wire.read();    // read one character from the I2C
//  if (x == 9)
//  {
//    On = 1;
//  }
//t = millis()/1000; 
//z = 1; 

 
}
 
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print("Read, x = ");
  //Serial.println(x);
  //Serial.print("Time = "); 
  //Serial.println(t);
  //if (On == 1)

  
if (On == 1)
{ 
 //pollFrontIR();
 delay(25);
  //Serial.print("X = "); 
  //Serial.println(x);  
 

//If x == 0, Do Nothing//

  if (x == 1)
  {
   //Serial.println("90 Deg Left");
   SnapL(); 
   x = 0; 
   
  }

  else if(x == 2)
  {
  // Serial.println("Slight Left");
   SL(); 
  x = 0; 
  }
  else if(x == 3)
  {
  //Serial.println("Go Straight");
   FORWARD();
   x = 0;
  }
   else if(x == 4)
  {
  // Serial.println("Slight Right");
   SR(); 
    x = 0;
  }
   else if(x == 5)
  {
   //Serial.println("90 Deg Right");
   SnapR(); 
   x = 0;
  }
  else if(x == 6)
  {
  //  Serial.println("Seek");
    SEEK(); 
    x = 0;
    
  }
   else if(x == 7)
  {
  //  Serial.println("STOP");
    //STOP(); 
    //turnOFF(); 
  }

  else if(x == 8)
  {
    //CUSTOM TEST LOOP//
    //SnapR();
    //STOP();
    delay(10);
    SnapL();
//    STOP();
    delay(20000);
    
  }
  
  }
  //pollFrontIR();
  delay(5); 
}


  void SEEK()
  {
y = y+1; 

    if (y == 2)
    {
       //Snap 45 Deg Right
motor1.setSpeed(-Out);
motor2.setSpeed(-Out);
motor3.setSpeed(-Out);
motor4.setSpeed(-Out);
delay(175);
motor1.setSpeed(0);
motor2.setSpeed(0);
motor3.setSpeed(0);
motor4.setSpeed(0);
//STOP();
//pollFrontIR();


    }

  else if (y == 4)
  {
  
motor1.setSpeed(Out);
motor2.setSpeed(Out);
motor3.setSpeed(Out);
motor4.setSpeed(Out); 
delay(175);
//STOP(); 
motor1.setSpeed(0);
motor2.setSpeed(0);
motor3.setSpeed(0);
motor4.setSpeed(0);

//pollFrontIR();


  }

  else if (y == 6)

  {
    motor1.setSpeed(Out);
motor2.setSpeed(Out);
motor3.setSpeed(Out);
motor4.setSpeed(Out); 
delay(175);
//STOP(); 
motor1.setSpeed(0);
motor2.setSpeed(0);
motor3.setSpeed(0);
motor4.setSpeed(0);
    y = 0; 
  //  pollFrontIR(); 
   
    
  }
    
 
}
  
  void FORWARD(){
motor1.setSpeed(225);
motor2.setSpeed(-225);
motor3.setSpeed(-225);
motor4.setSpeed(225);
//Serial.println("Forward"); 
//pollFrontIR(); 


  }

  void SL(){

motor1.setSpeed(120);//180
motor2.setSpeed(-150); //225
motor3.setSpeed(-150);
motor4.setSpeed(120); 
//Serial.println("SL"); 
//pollFrontIR();  
 
    
  }

  void SnapL(){
  
motor1.setSpeed(Out);
motor2.setSpeed(Out);
motor3.setSpeed(Out);
motor4.setSpeed(Out); 
//Serial.println("SnapL"); 
delay(scale);
STOP();


  }

  void SnapR(){
  
motor1.setSpeed(-Out);
motor2.setSpeed(-Out);
motor3.setSpeed(-Out);
motor4.setSpeed(-Out); 
//Serial.println("SnapL"); 
delay(scale);
STOP();

  }

  void SR(){

motor1.setSpeed(150);
motor2.setSpeed(-120);
motor3.setSpeed(-120);
motor4.setSpeed(150); 
//Serial.println("SR"); 
//pollFrontIR();  

    
  }

    
  
  

  void REVERSE(){
motor1.setSpeed(-Out);
motor2.setSpeed(Out);
motor3.setSpeed(Out);
motor4.setSpeed(-Out);  
//Serial.println("Reverse"); 
  
    }

  void STOP(){ 
motor1.setSpeed(0);
motor2.setSpeed(0);
motor3.setSpeed(0);
motor4.setSpeed(0);
//Serial.println("STOP"); 
}

  void FLIP(){
motor1.setSpeed(-Out);
motor2.setSpeed(-Out);
motor3.setSpeed(-Out);
motor4.setSpeed(-Out);
delay(650);
//Serial.println("FLIP");
STOP();

    
  }

  void turnOFF()
  {
  delay(50); 
  turnOFF(); 
  }

  
  ///////////////////////POLLING THE FRONT IR SENSORS//////////////////////////////////
void pollFrontIR() 
{

//240 ~ Avg Black
//140 ~ Avg White

//IR Sensor Measurement - SUBJECT TO CHANGE//


 U = 0;
 
irSen1 = analogRead(irPin1); 
//Serial.println(irSen1); 
  if (irSen1 < 80) 
    { 
      U = 1; 
    //  Serial.println("IR1(FL), White Line Detected, REVERSE");
         STOP(); 
       delay(10); 
       REVERSE(); 
       delay(500); 
       STOP();
      
       
      
      
    }

  else if (irSen1 >= 80) 
  {
  //  Serial.println("IR1(FR), On Black, Continue");
  }

if (U == 0)
{
irSen2 = analogRead(irPin2);
//Serial.println(irSen2); 
delay(10); 

  if (irSen2 <= 80) 
    { 
  //    Serial.println("IR2(FL), White Line Detected, REVERSE");
       STOP(); 
       delay(10); 
       REVERSE(); 
       delay(500); 
       STOP(); 
     
       
      

 
    
}
}
//pollFrontIR(); 
//  else if (irSen2 >= 205) 
//  {
//    Serial.println("IR2(FL), On Black, Continue");
//  }


}



///////////////////////POLLING THE REAR IR SENSORS//////////////////////////////////
/* Scratched Functionality
void pollRearIR() 
{ 
  //Measure Level//
  
irSen3 = analogRead(irPin3); 

 if (irSen3 < 205) 
    { 
      //Serial.println("IR3(RL), White Line Detected, FORWARD");
      STOP(); 
      delay(10); 
      FORWARD(); 
      delay(350); 
      STOP(); 
      loop();
       
        
    }

  else if (irSen3 >= 205) 
  {
    //Serial.println("IR3(RL), On Black, Continue");
  }
  


irSen4 = analogRead(irPin4); 

 if (irSen4 < 205) 
    { 
     // Serial.println("IR4(RR), White Line Detected, FORWARD");
        STOP(); 
      delay(10); 
      FORWARD(); 
      delay(350); 
      STOP(); 
      loop();
       
        
    }

  else if (irSen4 >= 205) 
  {
  //  Serial.println("IR4(RR), On Black, Continue");
  }
  


 
}
*/
 void PrintIR()
 {
   //Printing the Output//

  Serial.println("///////////////////////////////");
     
  Serial.print("IR1: ");
  Serial.print(irSen1); 
  Serial.println(" Lvl");

   Serial.print("IR2: ");
  Serial.print(irSen2); 
  Serial.println(" Lvl");

 delay(2000);  //Used to aid in readability of serial monitor. 
 
//  Serial.println("///////////////////////////////");
//     
//  Serial.print("IR3: ");
//  Serial.print(irSen3); 
//  Serial.println(" Lvl");
//
//   Serial.print("IR4: ");
//  Serial.print(irSen4); 
//  Serial.println(" Lvl");

 delay(2000);  //Used to aid in readability of serial monitor. 

 }


 
 
 
  
