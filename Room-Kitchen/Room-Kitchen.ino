/* Code for Secondary Microcontroller that handles Room and Kitchen functions.                      
 *  Following is the command list:
 *  
 *  RL1O - Room Light 1 On
 *  RL1F - Room Light 2 On
 *  RL2O - Room Light 1 On
 *  RL2F - Room Light 2 On
 *  RLS - Room Lights Status
 *  KS - Kitchen Status
 *  
 *  Note: ':' is used as Command Termination Character
 */
//Pin 10 and 11 for Relay
#define light1  10
#define light2 11

//Pins 2-7 for Ultrasonic Sesnors
#define tp1 2
#define ep1 3
#define tp2 4
#define ep2 5
#define tp3 6
#define ep3 7

//Flags to maintain status of lights
int flag1 = 0;
int flag2 = 0;


String str = ""; //Variable to accumulate command

char ch = ' '; // Variable to receive each character

boolean cmdAvailable = false; // To see if a command is available for execution
boolean cmdOK = false; // To check if have the right command and if it was executed



//Function to Convert the duration to distance
long microToCms(long microseconds) {
  return microseconds / 29 / 2;
}

void setup() {
  //Beginning serial at 9600 baud
  Serial.begin(9600);
  // Connection established and working!
  Serial.println("Wassup, Bella?"); 
  
  //Setting relay pins as Output
  pinMode(light1, OUTPUT);       
  pinMode(light2, OUTPUT);

  //Setting triggerpins as Output
  pinMode(tp1, OUTPUT); 
  pinMode(tp2, OUTPUT); 
  pinMode(tp3, OUTPUT);

  //Setting echopins as Input
  pinMode(ep1, INPUT);
  pinMode(ep2, INPUT);
  pinMode(ep3, INPUT); 

}

void loop() {
  // If serial is transmitting
  if(Serial.available() > 0) {
    while(Serial.available()) {
      ch = Serial.read(); // Read a byte from the Serial buffer
      delay(5);
      if(ch == ':') {
        cmdAvailable = true;
        break;
      }
      else { 
        str += ch;
      }
      delay(5);
    }
       // Serial.println(str);
    delay(1000);
  }// if serial available
    if(cmdAvailable) {
         //Turn on Room 1 Light 
        if(str.equals("RL1O")) {
          if(flag1) {
            Serial.println("F1:"); // light is already on
            delay(10);
            cmdOK = true;
          }
          else {
            Serial.println("T1:");
            flag1 = 1; // Change flag status
            //Relay Instruction
            digitalWrite(light1,LOW);          
           // Serial.println("Light ON");
            delay(10);
            cmdOK = true;
          }
        }
        //Turn off Room Light 1 
        if(str.equals("RL1F")) {
          if(!flag1) { //light 1 is already off
            Serial.println("T4:");
            delay(10);
            cmdOK = true;
          }
          else {
            Serial.println("F4:");
            delay(10);
            flag1 = 0;
            //Relay Instruction
            digitalWrite(light1,HIGH);          
            Serial.println("Light OFF");
            delay(10);
            cmdOK = true;
          }
        }
        //Turn on Room Light 2   
        if(str.equals("RL2O")) {
          if(flag2) {          //light 1 is already on
            Serial.println("F2:");
            delay(10);
            cmdOK = true;
               
          }
          else {
            Serial.println("T2:");
            delay(10);
            flag2 = 1;
            //Relay Instruction
            digitalWrite(light2,LOW);          
            Serial.println("Light ON");
            delay(10);
            cmdOK = true;
          }
        }
        //Turn off Room Light 2 
        if(str.equals("RL2F:")) {
          if(!flag2) {       //light 2 is already off
            Serial.println("F5");
            delay(10);
            cmdOK = true;
          }
          else {
            Serial.println("T5:");
            delay(10);
            flag2 = 0;
            //Relay Instruction
            digitalWrite(light2,HIGH);          
            Serial.println("Light OFF");
            delay(10);
            cmdOK = true;
          }
        }
        if(str.equals("RLS:")) {
          Serial.println(flag1);
          delay(10);
          Serial.println(flag2);
          delay(10);
          cmdOK = true;
        }

        // Get the Kitchen Status
        if(str == "KS:") {
          long d1,d2,d3,cm1,cm2,cm3;
          int len = 100; //Length of Box. Assumed value for now. Update after boxes are obtained
          int p1,p2,p3; //To calculate percentage of grocery
  
          digitalWrite(tp1, LOW); //low pulse first to ensure a clean high pulse.
          delayMicroseconds(2);
  
          digitalWrite(tp1, HIGH);
          delayMicroseconds(10);
  
          digitalWrite(tp1, LOW);

         // Read the signal from the sensor: a HIGH pulse whose
        // duration is the time (in microseconds) from the sending
        // of the ping to the reception of its echo off of an object.
  
          d1 = pulseIn(ep1, HIGH);

          // convert the time into a distance
          cm1 = microToCms(d1);

          //calculate percentage
          p1=(cm1/len)*100;
  
          Serial.println(p1);
          //Serial.print("cm");
          //Serial.println();
  
          delay(100);

    
          digitalWrite(tp2, LOW); //low pulse first to ensure a clean high pulse.
          delayMicroseconds(2);
  
          digitalWrite(tp2, HIGH);
          delayMicroseconds(10);
  
          digitalWrite(tp2, LOW);

    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
  
          d2 = pulseIn(ep2, HIGH);

    // convert the time into a distance
          cm2 = microToCms(d2);

          //calculate percentage
          p2=(cm2/len)*100;
  
          Serial.println(p2);
    //Serial.print("cm");
    //Serial.println();
  
          delay(100);

          digitalWrite(tp3, LOW); //low pulse first to ensure a clean high pulse.
          delayMicroseconds(2);
  
          digitalWrite(tp3, HIGH);
          delayMicroseconds(10);
  
          digitalWrite(tp3, LOW);

    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
  
          d3 = pulseIn(ep3, HIGH);

    // convert the time into a distance
          cm3 = microToCms(d3);

          //calculate percentage
          p3=(cm3/len)*100;
          
          Serial.println(p3);
    //Serial.print("cm");
    //Serial.println();
  
          delay(100);
          cmdOK = true;
             
        }
          if(str.equals("GSS")) {
          Serial.println("M175:");
          cmdOK = true;
        }
        cmdAvailable = false; // For next iteration
        str=""; // Empty the string for next command
        //cmdOK ? Serial.println("Command executed") : Serial.println("Error: wrong command issued"); // Command execution prompt

        // Get garden status (tentative code)
      
    }// If cmd available   
    cmdOK = false; // For next iteration
    
}// void loop

