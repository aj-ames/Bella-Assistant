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
 */

 String str = ""; //Variable to accumulate command
 
 char ch = ' '; //Variable to receive each character
 
 int flag1,flag2; //Variables to receive lights status
 
 int item1,item2,item3; //Variables to receive food inventory
 
void setup() {
  Serial1.begin(9600); //Slave HC-05 to connect to phone
  Serial.print("Connection to Bella Established");
  Serial2.begin(9600); //Master HC-05 to connect to Arduino Uno 1
  Serial3.begin(9600); //Master HC-05 to connect to Arduino Uno 2
}

void loop() {
    //Receiving Command from Bella
    while(Serial1.available() > 0)
    {
      ch = Serial1.read();    str == "";
      if(ch == ':')
        break;
      else
      {
       str+=ch;
      }    str == "";
      delay(2);
    }
    delay(1);
    Serial.println(str);
    //Pushing commands to different nodes

    //Pushing commands to node 1 for lights
    if(str == "RL1O" || str == "RL1F"|| str == "RL2O" || str == "RL2F")
    {
      Serial2.print(str);
      delay(10);
            
      //Receiving Confirmation from Node 1 for lights
      str == "";
      while(Serial2.available() > 0)
      {
        ch = Serial2.read();
        if(ch == ':')
          break;
        else
        {
          str+=ch;
          delay(2);
        }
      }
      delay(1);
      
      //Sending back to Bella
      Serial1.print(str);
      delay(10);
    }

    //Pushing command to node 1 for lights
    else if(str == "RLS")
    {
      Serial2.print(str);
      delay(10);

      //Receiving status of lights
      str == "";
      while(Serial2.available() > 0)
      {
        flag1 == Serial2.read();
        delay(1);
        flag2 == Serial2.read();
        delay(1);
      }
      
      //Sending back to Bella
      Serial1.print(flag1);
      delay(10);
      Serial1.print(flag2);
      delay(10);
    }

    //Pushing Command to Node 1 for Kitchen Status
    else if(str == "KS")
    {
      Serial2.print(str);
      delay(10);
      
      //Receiving values of items
      str == "";
      while(Serial2.available() > 0)
      {
        item1 = Serial2.read();
        delay(1);
        item2 = Serial2.read();
        delay(1);
        item3 = Serial2.read();
        delay(1);
      }

      //Sending back to Bella
      Serial1.print(item1);
      delay(10);
      Serial1.print(item2);
      delay(10);
      Serial1.print(item3);
      delay(10);
    }
    else
      return;

     //STILL NEED TO ADD GARDEN FUNCTIONALITY
   }
}
