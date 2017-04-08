 /* 
 *  
 *  Code for the Arduino which will control the garden's automatic watering system
 *  This will receive the commands from the Arduino mega, the main controller
 *  Command list:
 *  GSS - Garden status; return the moisture
 *  GSO - Garden sprinkler turned ON
 *  GSF - Garden sprinkler turned OFF
 *  All the commands will have a delimiter at the end of the command, the delimiter being ":" 
 *  Acknowlegdement sent:
 *  T3 - Turned ON the sprinkler
 *  F3 - Warning: the soil is too wet to be watered
 *  F6 - Remind the user that he/she has to water his/her plants; through Bella
 *  F7 - The sprinkler is already turned off
 *  M1XY - Moisture content when status is asked for; where XY is the percentage moisture
 *  
 */

#include<Servo.h>

#define potPosition 60
#define wateringTime 40
#define triggerMiostureContent 200
#define warningMoistureContent 750

//Servo Object to control the servo motor
Servo servo1;

// Define the pin for moisture sensor
const int moisturePin = A0;

//The value that which will be read from the moisture sensor
int moistureValue = 0;

//The average moisture value
long int moistureAvg = 0;

//Pins to use the Sprinkler Motor
const int motorPin1 = 2;
const int motorPin2 = 3;
const int motorEn = 6;

//The Arduino's LED pin
const int ledPin = 13; // pin that turns on the LED

//Command variables
String cmd = "";
String ch = "";
boolean cmdAvailable = false;
String delimiter = ":";
boolean sprinkler = false; // Flag 


void setup() {
   //Beginning Serial at 9600 Buad
   Serial.begin(9600);
   
   //Message displayed on succesfull connection
   Serial.println("Connection is up!");
   
   servo1.attach(8);  // attaches the servo on pin 8 to the servo object 
   initPosition();
   delay(500);
   
   pinMode(motorPin1, OUTPUT);
   pinMode(motorPin2, OUTPUT);
   pinMode(motorEn, OUTPUT);
   pinMode(moisturePin, INPUT);
 }


 void loop() {
  if(Serial.available() > 0) {
    while(Serial.available()) {
      ch = Serial.read();
      delay(5);
      if(ch == delimiter) {
        cmdAvailable = true;
        break;
      }
      else
        cmd += ch;
    }
  }
  //Serial.println(cmd);
  if(cmdAvailable) {
     if(cmd.equals("GSS")) {
        moistureAvg = moistureSampler();
        moistureAvg = (moistureAvg / 1024) * 100; // Calculate the percentage, for dear Bella *_*
        moistureAvg = moistureAvg - 100; //To reverse the value
        delay(100); //Just hold on a sec...
        if(moistureAvg < 10) {
          Serial.print("M10");
          Serial.println(moistureAvg);
        }
        else {
         Serial.print("M1");
         Serial.println(moistureAvg);
         moistureAvg = 0; // Reset the value after printing 
        }
     }
     if(cmd.equals("GSO")) {
        moistureAvg = moistureSampler();
        moistureAvg = (moistureAvg / 1024) * 100; // Calculate the percentage, for dear Bella *_*
        moistureAvg = moistureAvg - 100; //To reverse the value
        delay(100); //Just hold on a sec...
        if(moistureAvg >= 80) {
          Serial.print("F3:");//Soil is too wet to be watered
        }
        else  {
          startSprinkler();
          Serial.println("T3:");
          initPosition();
        }
     }  
     if(cmd.equals("GSF")) {
        fail();
     }
     if(cmd.equals("X")) {
      if(sprinkler) {
        return "T:";
      }
      else {
        return "F:";
      }
     }
     cmdAvailable = false;
  }// if command available

 }

int moistureSampler() {
  int sum = 0;
  for(int i = 0; i < 25; i++) {
    sum += analogRead(moisturePin);
    delay(20);
  }
  return (sum / 25);
}

void startSprinkler() {
  for(int i = 0; i < 10; i++) {
    servo1.write(0);// For Position at 0 degrees
    digitalWrite(motorEn,HIGH);
    digitalWrite(motorPin1,HIGH);
    digitalWrite(motorPin2,LOW);
    delay(500);
    servo1.write(90);// For Position at 90 degrees
    digitalWrite(motorEn,HIGH);
    digitalWrite(motorPin1,HIGH);
    digitalWrite(motorPin2,LOW);
    delay(500);
    servo1.write(180);// For Position at 180 degrees
    digitalWrite(motorEn,HIGH);
    digitalWrite(motorPin1,HIGH);
    digitalWrite(motorPin2,LOW);
    delay(500);
  }
}

void fail() {
  Serial.println("F7:");
}


void initPosition() {
   servo1.write(90); // set the servo to mid-point
   delay(500);  
 }





