/* Get commands from an Android phone and test them
 *  this is purely for the purpose of testing the transmission between the devices
 *  Different combinations of Leds statsus will be used for each command to check if the working can be emulated with the real hardware
 */


 const int ledPin = 13; //Arduion's LED pin number

 const int ledPin1 = 3; //
 const int ledPin2 = 4; //
 const int ledPin3 = 5; // 

 String cmd = ""; // The command string;
 char END_OF_CMD = ":"; // The end of command character
 char ch = "";
 boolean cmdComplete = false; // flag to check for the integrity of the command

 void setup() {
 
  pinMode(ledPin, OUTPUT); 
  digitalWrite(ledPin, LOW);
  
  pinMode(ledPin1, OUTPUT); 
  pinMode(ledPin2, OUTPUT); 
  pinMode(ledPin3, OUTPUT); 
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);

  // Start serial connection
  Serial.begin(9600);
  Serial.println("Connection working");

  }

  void loop() {
    //Check if Serial is transmitting
  if(Serial.available() > 0)
  {
    cmd = "";
    //To accumulate the command
      ch = Serial.read();
      delay(1);
      cmd += ch;
  }
  else {
    return;
  }

      Serial.println(cmd);
      boolean cmdOK = false;

      if(cmd.equals("1")) {
        digitalWrite(ledPin1, HIGH);
        cmdOK = true;
      }
      else if(cmd.equals("2")) {
        digitalWrite(ledPin1, LOW);
        cmdOK = true;
      }
      else if(cmd.equals("3")) {
        digitalWrite(ledPin2, HIGH);
        cmdOK = true;
      }
      else if(cmd.equals("4")) {
        digitalWrite(ledPin2, LOW);
        cmdOK = true;
      }
      else if(cmd.equals("5")) {
        Serial.print(digitalRead(ledPin1));
        Serial.print(":");// Values are sepreated by the char ":" for convenience
        Serial.println(digitalRead(ledPin2));
        cmdOK = true;
      }
      else if(cmd.equals("6")) {
        digitalWrite(ledPin3, HIGH);
        cmdOK = true;
      }
      else if(cmd.equals("7")) {
        digitalWrite(ledPin3, LOW);
        cmdOK = true;
      }
      else if(cmd.equals("8")) {
        Serial.print("The moisture status is: ");
        Serial.println(digitalRead(ledPin3));
        cmdOK = true;
      }
      else if(cmd.equals("9")) {
        digitalWrite(ledPin1, HIGH);
        delay(1000);
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, HIGH);
        delay(1000);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, HIGH);
        delay(1000);
        digitalWrite(ledPin3, LOW);
        cmdOK = true;
      }
    cmdOK ? Serial.println("Command Executed") : Serial.println("Invalid Command"); // Command flag
   
    //Clear command for next iteration
    cmd = "";
    cmdComplete = false;
  }



 
