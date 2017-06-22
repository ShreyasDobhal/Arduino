int led=13;
int val=0;
int dump=0;
void setup()
{
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  Serial.begin(9600);
}//setup()
void loop()
{
  if (Serial.available()>0)
  {
    val=Serial.read();
    dump=Serial.read();
    Serial.println(val);
  }//if block
  if (val=='1'||dump=='1')//||val=="on")
  {
    digitalWrite(led,HIGH);
    Serial.println("LED On");
    val=0;
    dump=0;
  }
  else if(val=='0'||dump=='0')//||val=="off")
  {
    digitalWrite(led,LOW);
    Serial.println("LED Off");
    val=0;
    dump=0;
  }
  delay(100);
}//loop()
