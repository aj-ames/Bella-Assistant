/* Code for the Arduino which will control the garden's automatic watering system
 *  This will receive the commands from the Arduino mega, the main controller
 *  Command list:
 *  GSS - Garden status; return the moisture
 *  GSO - Garden sprinkler ON
 *  GSF - Garden sprinkler OFF
 *  All the commands will have a delimiter at the end of the command, the delimiter being ":" 
 *  Acknowlegdement sent:
 *  T3 - Turned ON the sprinkler
 *  T6 - Turned OFF the sprinkler
 *  F3 - Sprinklers already turned ON
 *  F8 -  Sprinklers already turned OFF
 *  M1XY - Moisture content when status is asked for; where XY is the %
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


 void setup() {
  //Beginning Serial at 9600 Buad
  Serial.begin(9600);
  //Message displayed on succesfull connection
  Serial.println("Connection is up!");
  
  
  
 }
 
