// Master_V1
// Created by: Tristan Kuzma
// 05/08/22
// Version 2, Created 05/21/22
// MIME498 - MIME CAPSTONE DESIGN
// TEAM 605.1 Sumobot

//IR Sensor Measurements will get moved to the Slave System in Master V3, Slave V2. This cannot occur properly until motor drive systems can be calibrated. 

/// GLOBAL //////////////////////////////////////////////////////////////////////////////////////////


#include <Wire.h>   // Include Wire Library for I2C Communication
#include "NewPing.h" // Include NewPing Library for Ultrasonic Sensors


// Define Variables

int x = 0;  //Used in Arduino Communication via I2C Bus

//US Read Duration
float duration1;
float duration2;
float duration3;
float duration4;
float duration5;

//Dur to Dist
float distance1;
float distance2;
float distance3;
float distance4;
float distance5;

//Ultrasonic Logic

//IR Sensors - Subject to change.
float irSen1;
float irSen2;
float irSen3;
float irSen4;


// Define Constants

//Ultrasonic
float dur2dist = 0.017; //Variable that is the speed of sound in m/s divided by 10000, divided by 2. 
int repeat = 4; //How many times are we repeating to find median???
#define trigPin1 4
#define trigPin2 5
#define trigPin3 6
#define trigPin4 7
#define trigPin5 8
#define echoPin1 4
#define echoPin2 5
#define echoPin3 6
#define echoPin4 7
#define echoPin5 8
#define maxDistance 300 //Measurments above this are an error.

//IR Sensors
#define irPin1 A0 
#define irPin2 A1
#define irPin3 A2 
#define irPin4 A3

#define irPinLED1 9
#define irPinLED2 10
#define irPinLED3 11
#define irPinLED4 12

//LED
#define powerLED 13

//Button
#define Button 2

//New Ping Functions for Ultrasonics
NewPing sonar1(trigPin1, echoPin1, maxDistance);
NewPing sonar2(trigPin2, echoPin2, maxDistance);
NewPing sonar3(trigPin3, echoPin3, maxDistance);
NewPing sonar4(trigPin4, echoPin4, maxDistance);
NewPing sonar5(trigPin5, echoPin5, maxDistance);

//Interupt Constants
volatile bool state_interrupt = false;  //Used in Interrupt
volatile long time_interrupt = 0; //Used in Debounce
int standardDelay = 200; //Used in debounce. 
int On = 1; //1 = On, 0 = Off. Used in Button Argument. 

//Timing 
unsigned long  T2;  //Ending Time 
unsigned long T1; //Start Time 
float Tdiff;  //Time to complete cycle. 
int y = 1;  //Iteration
 



/// SETUP //////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Master Initializing..."); // print some text in Serial Monitor

Wire.begin(); //Begins I2C Communication

//Interrupt
  pinMode(Button, INPUT);

  pinMode(powerLED, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(Button),On_Off, RISING);

//More IR Sensor Code
  pinMode(irPin1, INPUT);
    pinMode(irPin2, INPUT);
    
     pinMode(irPinLED1, OUTPUT);
      pinMode(irPinLED2, OUTPUT);
}

void On_Off() {
    if (!state_interrupt) {
      //state_interrupt = true;

      if (millis() - time_interrupt > standardDelay)
      {
      if (On == 0) {
      digitalWrite(powerLED, HIGH);
      On = 1; 
       time_interrupt = millis();
      
      }

      else if (On == 1) {
      digitalWrite(powerLED, LOW); 
      On = 0;
       time_interrupt = millis(); 
       x = 7; 
       coms(); 
       
      }
   
      
  }
    }

}

/// MAIN LOOP //////////////////////////////////////////////////////////////////////////////////////////
void loop() {
 if (On == 1)
 {
T1 = millis()/1000; 
//Ultrasonic Sensor Measurement//
 Serial.println("///////////////////////////////");
Serial.print("Iteration :");
Serial.println(y); 


  //Measure duration of wave travel//
  delay(40); //Break
  duration1 = sonar1.ping_median(repeat);
  delay(40); //Break
  duration2 = sonar2.ping_median(repeat);
  delay(40); //Break
  duration3 = sonar3.ping_median(repeat);
  delay(40); //Break
  duration4 = sonar4.ping_median(repeat);
  delay(40); //Break
  duration5 = sonar5.ping_median(repeat);

  //Measure distance of wave travel//
  distance1 = duration1 * dur2dist; 
    distance2 = duration2 * dur2dist; 
      distance3 = duration3 * dur2dist; 
        distance4 = duration4 * dur2dist; 
          distance5 = duration5 * dur2dist; 

 //Print the Ultrasonic Sensor Measurements//

   Serial.println("///////////////////////////////");

   Serial.println("Ultrasonic Sensor Measurements");
  Serial.print("S1(LS): ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("S2(LF): ");
  Serial.print(distance2);
  Serial.println(" cm");

   Serial.print("S3(CF): ");
  Serial.print(distance3);
  Serial.println(" cm");

   Serial.print("S4(RF): ");
  Serial.print(distance4);
  Serial.println(" cm");

   Serial.print("S5(RS): ");
  Serial.print(distance5);
  Serial.println(" cm");

  //Serial.print("Minimum: ");
  //Serial.print(minUSVal);
  //Serial.println(" cm");

//Logic Controls//

  //Minimum Distance Logic//

  int Range = 200; 
  float minUSVal = Range; 
    float USVals[5] = { distance1,  distance2,  distance3,  distance4,  distance5};
  // float minUSVal = distance1; 
   for (int i = 0; i < 5; i++) {

    if (USVals[i] > 0)  //Is the value greater than 0? 
      { 
      if ( USVals[i] < minUSVal)  //Is the value less than the pre-defined minimun? 
        { 
          minUSVal = USVals[i]; //This is in range, iterate over the loop to find min. 
        }
      }
      
   }

   Serial.print("Min Val ="); 
   Serial.println(minUSVal);
   
 
   

   //Seek Command, [Out of Range], [Range = 0]//
  

   if (minUSVal >= Range)
    {
      Serial.println("Seeking -> No Opponent Detected");
      x = 6;

      coms();
    }

     if (minUSVal == 0)
    {
      Serial.println("Seeking -> No Opponent Detected");
      x = 6;

      coms();
    }

  //Opponent Found, Drive Direction//

//Control Guide 
// x = 0 -> Do Nothing 
// x = 1 -> 90 Degree Left
// x = 2 -> Slight Left
// x = 3 -> Center Drive
// x = 4 -> Slight Right
// x = 5 -> 90 Deg Right
// x = 6 -> Seek
// x = 7 -> Stop [Used in On_Off Function]
// x = 8 -> Reverse
  if (distance1 == minUSVal) 
  {
    x = 1;
    Serial.print("90 Deg Left");
    coms();
  }

  else if (distance2 == minUSVal) 
  {
    x = 2;
    Serial.println("Slight Left");
    coms();
  }
  
  else if (distance3 == minUSVal) 
  {
    x = 3;
     Serial.println("Go Straight");
     coms();
  }
  
  else if (distance4 == minUSVal) 
  { 
    x = 4;
    Serial.println("Slight Right");
    coms();
  }

   else if (distance5 == minUSVal) 
  {
    x = 5;
     Serial.println("90 Deg Right");
     coms();
  }

 
  }
}

///////////////////////COMMUNICATIONS FUNCTION FOR I2C BUS//////////////////////////////////
void coms() {
  
Wire.beginTransmission(9); // transmit to device #9
 Wire.write(x);              // sends x 
  Wire.endTransmission();    // stop transmitting

 T2 = millis()/1000;

Tdiff = T2 - T1; 

  Serial.println(Tdiff);

  y = y+1; 

  delay(5000);

  loop(); 
  
}

///////////////////////POLLING THE FRONT IR SENSORS//////////////////////////////////
void pollFrontIR() 
{

//240 ~ Avg Black
//140 ~ Avg White

//IR Sensor Measurement - SUBJECT TO CHANGE//

  //Measure Level//
  digitalWrite(irPinLED1, HIGH); 
  delay(10); //Break
irSen1 = analogRead(irPin1); 

  if (irSen1 < 200) 
    { 
      Serial.println("IR1, White Line Detected, REVERSE");
        x = 8;
        coms(); 
    }

  else if (irSen1 >= 200) 
  {
    Serial.println("IR1, On Black, Continue");
  }

digitalWrite(irPinLED2, HIGH); 
  delay(10); //Break
irSen2 = analogRead(irPin2); 

  if (irSen2 < 200) 
    { 
      Serial.println("IR2, White Line Detected, REVERSE");
        x = 8;
        coms(); 
    }

  else if (irSen2 >= 200) 
  {
    Serial.println("IR2, On Black, Continue");
  }




  //Printing the Output//

  Serial.println("///////////////////////////////");
     
  Serial.print("IR1: ");
  Serial.print(irSen1); 
  Serial.println(" Lvl");

   Serial.print("IR2: ");
  Serial.print(irSen2); 
  Serial.println(" Lvl");

 delay(2000);  //Used to aid in readability of serial monitor. 
 
}

///////////////////////POLLING THE REAR IR SENSORS//////////////////////////////////
void pollRearIR() 
{ 
  //Measure Level//
  digitalWrite(irPinLED3, HIGH); 
  delay(10); //Break
irSen3 = analogRead(irPin3); 

 if (irSen3 < 200) 
    { 
      Serial.println("IR3, White Line Detected, FORWARD");
        x = 5;
        coms(); 
    }

  else if (irSen1 >= 200) 
  {
    Serial.println("IR3, On Black, Continue");
  }
  

digitalWrite(irPinLED4, HIGH); 
  delay(10); //Break
irSen4 = analogRead(irPin4); 

 if (irSen4 < 200) 
    { 
      Serial.println("IR4, White Line Detected, FORWARD");
        x = 5;
        coms(); 
    }

  else if (irSen4 >= 200) 
  {
    Serial.println("IR4, On Black, Continue");
  }
  


  //Printing the Output//

  Serial.println("///////////////////////////////");
     
  Serial.print("IR3: ");
  Serial.print(irSen3); 
  Serial.println(" Lvl");

   Serial.print("IR4: ");
  Serial.print(irSen4); 
  Serial.println(" Lvl");

 delay(2000);  //Used to aid in readability of serial monitor. 
  
}
 
