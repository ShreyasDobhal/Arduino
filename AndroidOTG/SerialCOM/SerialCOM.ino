String val="";
void setup()
{
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  Serial.begin(9600);
}
void loop()
{
  while(Serial.available()>0)
  {
    val=val+(char)Serial.read();
  }
  if (val!="")
    Serial.println(val);
  if (val=="1"||val=="on")
  {
    val="";
    digitalWrite(13,HIGH);
  }
  else if (val=="0"||val=="off")
  {
    val="";
    digitalWrite(13,LOW);
  }
  else if (val=="hello")
  {
    Serial.println("hi");
    val="";
  }
  delay(100);
}
