/* Code to run on the Server Arduino Board.
 *  This code will redirect the command code issued by Bella to the appropriate child node.
 *  
 *  Commands: 
 *  
 *  To be sent to Arduino Uno 1:
 *  RL1O - Room Light 1 On
 *  RL1F - Room Light 2 On
 *  RL2O - Room Light 1 On
 *  RL2F - Room Light 2 On
 *  RLS - Room Lights Status
 *  KS - Kitchen Status
 *  
 *  To be sent to phone:
 *  F - Room Light Already On/Off
 *  T - Room Light Turned On/Off
 *  
 *  To be sent to Arduino Uno 2:
 *  GSS - Garden status; return the moisture
 *  GSO - Garden sprinkler turned ON
 *  GSF - Garden sprinkler turned OFF
 *  
 *  Serial0 is used to view on Serial Monitor
 *  Serial1 is used for Phone-Server Connection; The phone
 *  Serial2 is used for Server-Node1 Connection; The Room and Kitchen
 *  Serial3 is used for Server-Node2 Connection; The Garden
 *  
 *  P.S. All commands are followed by ':' character to mark Command Termination
 *  
 *  Make sure to set the board as "Arduino/Genuino Mega 2560" before compiling.
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

//Variable to accumulate command and information
String cmd = "", stat = "";

//For each character
char ch = ''; 

//For the food inventory
String item1, item2, item3;

//Flags
boolean cmdAvailable = false;
boolean flag1 = false;
boolean flag2 = false;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  setupFunc();
}

void loop() {
  //Get the command from  Bella
  if(Serial1.available() > 0) {
    cmd = "";
    while(Serial1.available()) {
      ch = Serial1.read();
      delay(5);
      if(ch == delimiter) {
        cmdAvailable = true;
        cmd += ch;
        break;
      }
      else {
        cmd += ch;
      }
      delay(5);
    }
    Serial.println(cmd);
    Serial.flush();
  }

  //Getting the work done
  // Room kitchen
  if(cmd.equals("RL1O:") || cmd.equals("RL1F:") || cmd.equals("RL2O:") || cmd.equals("RL2F:") 
        || cmd.equals("KS:")) {
    roomKitchen(cmd);
  }
  // Garden
  if( cmd.equals("GSS:") || cmd.equals("GSO") || cmd.equals("GSF:") ) {
    garden(cmd);
  }
  //Get status
  if( cmd.equals("X") ) {
    getStatus();
  }
}// Void loop


void setupFunc() {
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

void roomKitchen(String cmd) {
   //Turn on Room 1 Light 
   if(str.equals("RL1O")) {
    if(flag1) {
      Serial1.println("F1:"); // light is already on
      Serial1.flush();
      Serial.println("F1:");
      Serial.flush();
     }
     else {
      Serial1.println("T1:");
      Serial1.flush();
      flag1 = true; // Change flag status
      //Relay Instruction
      digitalWrite(light1,LOW);
      Serial.println("T1:");          
      Serial.flush();
     }
    }
    //Turn off Room Light 1 
    if(str.equals("RL1F")) {
      if(!flag1) { //light 1 is already off
        Serial.println("F4:");
        Serial.flush();
        Serial1.println("F4:");
        Serial1.flush();
       }
       else {
        Serial.println("T4:");
        Serial.flush();;
        flag1 = false;
        //Relay Instruction
        digitalWrite(light1,HIGH);
        Serial.println("F4:");          
        Serial.flush();
        Serial1.println("F4:");
        Serial1.flush();
       }
    }
    //Turn on Room Light 2   
    if(str.equals("RL2O")) {
      if(flag2) {//light 2 is already on
        Serial.println("F2:");
        Serial.flush();
        Serial1.println("F2:");
        Serial1.flush();
       }
       else {
         Serial.println("T2:");
         Serial.flush();
         Serial1.println("F4:");
         Serial1.flush();
         flag2 = true;
         //Relay Instruction
         digitalWrite(light2,LOW);          
       }
      }
      //Turn off Room Light 2 
       if(str.equals("RL2F")) {
        if(!flag2) {       //light 2 is already off
          Serial.println("F5");
          Serial.flush();
          Serial.println("F5:");
          Serial.flush();
          cmdOK = true;
         }
         else {
          Serial.println("T5:");
          Serial.flush();
          Serial1.println("T5:");
          Serial1.flush();
          flag2 = false;
          //Relay Instruction
          digitalWrite(light2,HIGH);          
         }
       }
       if(str == "KS") {
          long d1,d2,d3,cm1,cm2,cm3;
          int len = 100; //Length of Box. Assumed value for now. Update after boxes are obtained
          int p1,p2,p3; //To calculate percentage of grocery
  
          digitalWrite(tp1, LOW); //low pulse first to ensure a clean high pulse.
          //delayMicroseconds(2);  
          digitalWrite(tp1, HIGH);
          //delayMicroseconds(10);
          digitalWrite(tp1, LOW);

         // Read the signal from the sensor: a HIGH pulse whose
        // duration is the time (in microseconds) from the sending
        // of the ping to the reception of its echo off of an object.
  
          d1 = pulseIn(ep1, HIGH);
          // convert the time into a distance
          cm1 = microToCms(d1);
          //calculate percentage
          p1=(cm1/len)*100;
          //delay(100);

          digitalWrite(tp2, LOW); //low pulse first to ensure a clean high pulse.
          //delayMicroseconds(2);  
          digitalWrite(tp2, HIGH);
          //delayMicroseconds(10);
          digitalWrite(tp2, LOW);

    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
  
          d2 = pulseIn(ep2, HIGH);

    // convert the time into a distance
          cm2 = microToCms(d2);
          //calculate percentage
          p2=(cm2/len)*100;
          //delay(100);
          digitalWrite(tp3, LOW); //low pulse first to ensure a clean high pulse.
          //delayMicroseconds(2);
          digitalWrite(tp3, HIGH);
          //delayMicroseconds(10);
          digitalWrite(tp3, LOW);

    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
  
          d3 = pulseIn(ep3, HIGH);
    // convert the time into a distance
          cm3 = microToCms(d3);
          //calculate percentage
          p3=(cm3/len)*100;
           // Send the recorded information
          if(p1 < 10) {
            Serial.print("C10");
            Serial.print(p1);
            Serial.flush();
            Serial1.print("C10");
            Serial1.print(p1);
            Serial1.flush();
          }
          else {
            Serial.print("C1");
            Serial.println(p1);
            Serial.flush();
            Serial1.print("C1");
            Serial1.println(p1);
            Serial1.flush();
          }
          
          if(p2 < 10) {
            Serial.print("C20");
            Serial.print(p2);
            Serial.flush();
            Serial1.print("C20");
            Serial1.println(p2);
            Serial1.flush();
          }
          else {
            Serial.print("C2");
            Serial.println(p2);
            Serial.flush();
            Serial1.print("C2");
            Serial1.println(p2);
            Serial1.flush(); 
          }
          
          if(p3 < 10) {
            Serial.print("C30");
            Serial.print(p3);
            Serial.flush();
            Serial1.print("C30");
            Serial1.print(p3);
            Serial1.flush();
          }
          else {
            Serial.print("C3");
            Serial.println(p3);
            Serial.flush();
            Serial1.print("C3");
            Serial1.println(p3);
            Serial1.flush();
          }    
}


