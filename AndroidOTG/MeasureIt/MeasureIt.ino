String val="";

int value=0;
// 5 - 100
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
  else if (val=="ok")
  {
    // measure the dist
    calc();
    Serial.println((char)(value+28));
    val="";
  }
  
  delay(100);
}
void calc()
{
  value=(int)(analogRead(A0)*1.0/1024*100);
  if (value<5)
    value=5;
  if (value>=100)
    value=99;
}
