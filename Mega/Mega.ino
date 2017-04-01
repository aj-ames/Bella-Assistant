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

//Variable to accumulate command and information

String cmd = "", stat = "";

//For each character
chat ch = ''; 

//For the food inventory
String item1, item2, item3;

//Flags
boolean cmdAvailable = false;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
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
  if(str.equals("RL1O:") || str.equals("RL1F:") || str.equals("RL2O:") || str.equals("RL2F:") 
        || str.equals("KS:")) {
    roomKitchen(cmd);
  }
}// Void loop

