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
 *  GSO - Garden Sprinker On
 *  GSF - Garden Sprinkler Off
 *  GS - Garden Moisture Status
 *  
 *  Serial0 is used to view on Serial Monitor
 *  Serial1 is used for Phone-Server Connection
 *  Serial2 is used for Server-Node1 Connection
 *  Serial3 is used for Server-Node2 Connection
 *  
 *  P.S. All commands are followed by ':' character to mark Command Termination
 *  
 *  Make sure to set the board as "Arduino/Genuino Mega 2560" before compiling.
 */

 String str = "", str2 = ""; //Variable to accumulate command
 
 char ch = ' '; //Variable to receive each character
 
 int flag1,flag2; //Variables to receive lights status
 
 int item1,item2,item3; //Variables to receive food inventory

 boolean cmdAvailable = false; //flag to check availability of command
 
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); //Slave HC-05 to connect to phone
  Serial2.begin(9600); //Master HC-05 to connect to Arduino Uno 1
  Serial3.begin(9600); //Master HC-05 to connect to Arduino Uno 2
}



void loop() {
  //Receviving Command from Bella
    if(Serial1.available() > 0) {
      str = "";
        while(Serial1.available()) { 
          ch = Serial1.read();
          delay(5);
          if(ch == ':') {
              cmdAvailable = true;
              str += ch;
              break;
          }
          else {
              str += ch;
          }
          delay(500);
        }
        Serial.println(str);
    }

    //Checking different command conditions

    //Commands related to room
    if(str.equals("RL1O:") || str.equals("RL1F:") || str.equals("RL2O:") || str.equals("RL2F:") ) {
        if(cmdAvailable) {  
          Serial2.println(str);
          delay(10);
          cmdAvailable = false;
        }
        
        if(Serial2.available() > 0) {        
            str2 = "";
            while(Serial2.available()) {
                ch = Serial2.read();
                delay(5);
                if(ch == ':') {
                    str2 += ch;
                    break;
                }
                else {
                    str2 += ch;
                }
            }
            Serial.println(str2);
            delay(10);
            //Sending to Bella
            Serial1.println(str2);
            delay(500);
        }
    }

    //Command to receive room status
    if(str.equals("RLS:")) {
        if(cmdAvailable) {   
          Serial2.println(str);
          delay(10);
          cmdAvailable = false;
        }

        //Receiving from Node 1
        if(Serial2.available() > 0) {
            flag1 = 0;
            flag2 = 0;
            while(Serial2.available()) {
                flag1 = Serial2.read();
                delay(10);
                flag2 = Serial2.read();
                delay(10);
            }
            //Sending to Bella
            Serial.println(flag1);
            delay(10);
            Serial.println(flag2);
            delay(10);
            Serial1.println(flag1);
            delay(10);
            Serial1.println(flag2);
            delay(10);
        }
    }

    //Command to recevive Kitchen Status
    if(str.equals("KS:")) {
        if(cmdAvailable) {
            Serial2.print(str);
            delay(10);
            cmdAvailable = false;
        }

        //Receiving from Node 1
        if(Serial2.available() > 0) {
            item1 = 0;
            item2 = 0;
            item3 = 0;
            while(Serial.available()) {
                item1 = Serial2.read();
                delay(10);
                item2 = Serial2.read();
                delay(10);
                item3 = Serial2.read();
                delay(10);
            }
            //Sending to Bella
            Serial.println(item1);
            delay(10);
            Serial.println(item2);
            delay(10);
            Serial.println(item3);
            delay(10);
            Serial1.println(item1);
            delay(10);
            Serial1.println(item2);
            delay(10);
            Serial1.println(item3);
            delay(10);
        }
    }

    //NEED TO ADD GARDEN FUNCTIONALITY
}//End of loop

