String str="";
char ch=' ';
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("Wassup, Bella ?");
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()>0)
  {
    //To accumulate the command
      ch=Serial.read();
      delay(10);
      if(ch == ':')
      {
      }
      else
      { 
          str+=ch;
      }
      delay(10);
  }
  Serial.print(str);
  delay(1000);
}
