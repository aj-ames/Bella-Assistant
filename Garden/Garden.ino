/* Code for the Arduino which will control the garden's automatic watering system
 *  This will receive the commands from the Arduino mega, the main controller
 *  Command list:
 *  GSS - Garden status; return the moisture
 *  GSO - Garden sprinkler ON
 *  GSF - Garden sprinklerforced to turn ON (will  work only for 10 seconds)
 *  All the commands will have a delimiter at the end of the command, the delimiter being ":" 
 *  Acknowlegdement sent:
 *  T3 - Turned ON the sprinkler
 *  T6 - Turned ON the sprinkler forcefully
 *  F3 - Sprinklers already turned ON, working currently
 *  F7 - Warning: the soil is too wet to be watered
 *  M1XY - Moisture content when status is asked for; where XY is the %
 *  
 *  The sprinkler flags:
 *  Throught the program we will mantain a flag variable called 'sprinkler'; this is just to maintain the exit status of the function 'startSprinkler()'
 *  The value of the flag can be as follows: 1, 2, 3, 4
 *  1 - The sprinkler was turned ON; for command GSO
 *  2 - The sprinkler was NOT turned ON because, the water is too wet; for command GSO
 *  3 - The sprinklers are working, for command GSO
 *  4 - The sprinkler was forced to turn ON; will work only for a period of 10 seconds, for command GSF
 */

#include<Servo.h>


Servo servo1; // Servo object to control the servo
// Define the pin
const int analogInPin0 = A0; 

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
char ch = '';
boolean cmdAvailable = false;
char delimiter = ':';
int sprinkler = 0;
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
     if(cmd.equals("GSS") {
        moistureAvg = sampleMoisture();
        Serial.print("M1");
        Serial.println(moistureAvg);
        moistureAvg = 0; // Reset the value after printing
     }
     if(cmd.equals("GSO")) {
        sprinkler = startSprinkler(force);
        switch(sprinkler) {
          case 1: Serial.println("T3:");
                  delay(5);
                  break;
          case 2: Serial.println("F7:");
                  delay(5);
                  break;
          case 3: Serial.println("F3:");
                  delay(5);
                  break;
          default:Serial.println("Error");
        }
     }
     if(cmd.equals("GSF")) {
        force = true;
        sprinkler = startSprinkler(force);
        if(sprinkler == 4) {
          Serial.println("T6:");
          delay(5);
        }
     }
     cmdAvailable = false;
  }// if command available
}//void loop

