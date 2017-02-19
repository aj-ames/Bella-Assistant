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
 *  T6 - Turned OFF the sprinkler forcefully
 *  F3 - Warning: the soil is too wet to be watered
 *  F6 - Remind the user that he/she has to water his/her plants; through Bella
 *  M1XY - Moisture content when status is asked for; where XY is the %
 *  
 */

#include<Servo.h>

#define potPosition 60
#define wateringTime 40
#define triggerMiostureContent 200
#define warningMoistureContent 950


Servo servo1; // Servo object to control the servo
// Define the pin
const int analogPin = A0; 

//The value that which will be read from the moisture sensor
int moistureValue = 0;

//The average moisture value
long int moistureAvg = 0;

//Define the water pump contorl pins
const int pumpAnodePin =  7;      //pin 7 connect to the anode of the pump
const int pumpCathodePin =  8;   //pin 8 connect to the cathode of the pump

//The Arduino's LED pin
const int ledPin = 13; // pin that turns on the LED

//Command variables
String cmd = "";
char ch = "";
boolean cmdAvailable = false;
char delimiter = ':';
boolean force = false; // Force start the motor


 void setup() {
   //Beginning Serial at 9600 Buad
   Serial.begin(9600);
   //Message displayed on succesfull connection
   Serial.println("Connection is up!");
   servo1.attach(10);  // attaches the servo on pin 10 to the servo object 
   initPosition();
   delay(500);
   pinMode(pumpAnodePin, OUTPUT);
   pinMode(pumpCathodePin, OUTPUT);
 }


 void initPosition() {
   servo1.write(90); // set the servo to mid-point
   delay(500);
   
 }


void loop() {
  delay(500);
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
  Serial.println(cmd);
  if(cmdAvailable) {
     if(cmd.equals("GSS")) {
        moistureAvg = moistureSampler();
        moistureAvg = (moistureAvg / 1024) * 100; // Calculate the percentage, for dear Bella *_*
        delay(1000); //Just hold on a sec...
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
        startSprinkler();
        Serial.println("T3:");
     }
     if(cmd.equals("GSF")) {
        stopSprinkler();
        Serial.println("T6:");
        
     }
     cmdAvailable = false;
  }// if command available
}//void loop


int moistureSampler() {
  int sum = 0;
  for(int i = 0; i < 25; i++) {
    sum += analogRead(analogPin);
    delay(1);
  }
  if(sum < 200)
    Serial.println("F6:");
  return sum / 25;
}


// Sprinkler controller function
void startSprinkler() {
  int moisture = moistureSampler();
  if(moisture > warningMoistureContent) {
    Serial.println("F3:");
    return; 
  }
  servo1.write(potPosition);  // setting the servo to the position of the flower
  delay(500); //waiting the servo go to right position
  digitalWrite(pumpAnodePin, HIGH);  //the pump shall start working
  digitalWrite(pumpCathodePin, LOW);
  for(int nos = 1; nos <= 10; nos += 1) {
    
    for(int pos = potPosition - 10; pos <= potPosition + 10; pos += 1) {
      servo1.write(pos);
      delay(wateringTime);
    }
    
    for(int pos = potPosition + 10; pos >= potPosition - 10; pos -= 1) {
      servo1.write(pos);
      delay(wateringTime);
    }
    
  }
  digitalWrite(pumpAnodePin, LOW); // Switch off the pump
  digitalWrite(pumpCathodePin, LOW);
  delay(500);
  initPosition(); // Come back to start position

}

void stopSprinkler() {
  digitalWrite(pumpAnodePin, LOW);
  digitalWrite(pumpCathodePin, LOW);
  delay(500);
  initPosition();
}

