/* Code for Secondary Microcontroller that handles Room and Kitchen functions.                      
 *  Following is the command list:
 *  
 *  RL1O - Room Light 1 On
 *  RL1F - Room Light 2 On
 *  RL2O - Room Light 1 On
 *  RL2F - Room Light 2 On
 *  RLS - Room Lights Status
 *  F - Room Light Already On/Off
 *  T - Room Light Turned On/Off
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

//Variables to check grocery items
int cm1 = 0, cm2 = 0, cm3 = 0;

String str = "";
char ch = ' ';

void setup(){ 
  //Beginning serial at 9600 baud
  Serial.begin(9600);

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

void loop(){
  //Check if Serial is transmitting
  if(Serial.available() > 0)
  {
    str = "";
    //To accumulate the command
    while(Serial.available())
    {
      ch=Serial.read();
      if(ch == ':')
      {
        break;
      }
      else
      { 
        str += ch;
      }
      delay(1);
    }
  }
  Serial.print("Recevied String - ");
  Serial.print(str);
  Serial.println();

  //Turn on Room 1 Light On
  if(str == "RL1O")
  {
    if(flag1 == 1)          //light 1 is already on
      Serial.print('F');
     else
     {
      Serial.print('T');
      flag1 == 1;
      //Relay Instruction
      digitalWrite(light1,LOW);          
      Serial.println("Light ON");
      delay(2000);
     }
  }

  //Turn on Room Light 1 Off
  if(str == "RL1F")
  {
    if(flag1 == 0)          //light 1 is already off
      Serial.print('F');
     else
     {
      Serial.print('T');
      flag1 == 0;

      //Relay Instruction
      digitalWrite(light1,HIGH);          
      Serial.println("Light OFF");
      delay(2000);
     }
  }

  //Turn on Room Light 2  On
  if(str == "RL2O")
  {
    if(flag2 == 1)          //Light 2 is already on
      Serial.print('F');
     else
     {
      Serial.print('T');
      flag2 == 1;
      //Relay Instruction
      digitalWrite(light2,LOW);          
      Serial.println("Light ON");
      delay(2000);
     }
  }

  //Turn off Room Light 2 Off
  if(str == "RL1F")
  {
    if(flag2 == 0)          //Light 2 is already off
      Serial.print('F');
     else
     {
      Serial.print('T');
      flag2 == 0;
      //Relay Instruction
      digitalWrite(light2,HIGH);          
      Serial.println("Light OFF");
      delay(2000);
     }
  }

  //To send back status of Lights
  if(str == "RLS")
  {
    Serial.print(flag1);
    delay(10);
    Serial.print(flag2);
    delay(10);
  }

  if(str == "KS")
  {
    long d1,d2,d3,cm1,cm2,cm3;
  
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
    cm1 = microsecondsToCentimeters(d1);
  
    Serial.print(cm1);
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
    cm2 = microsecondsToCentimeters(d2);
  
    Serial.print(cm2);
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
    cm3 = microsecondsToCentimeters(d3);
  
    Serial.print(cm3);
    //Serial.print("cm");
    //Serial.println();
  
    delay(100);
  }
}

//Function to Convert the duration to distance
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
