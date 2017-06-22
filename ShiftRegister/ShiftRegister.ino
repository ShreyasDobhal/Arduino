
int DS_pin = 8;
int STCP_pin = 9;
int SHCP_pin = 10;
boolean registers[16];

void setup()
{
  pinMode(DS_pin,OUTPUT);
  pinMode(STCP_pin,OUTPUT);
  pinMode(SHCP_pin,OUTPUT);
  //writereg();
  for (int i=0;i<16;i++)
    registers[i]=LOW;
  delay(100);
  writereg();
}

void writereg()
{
  for (int i=0;i<16;i++)
  {
    digitalWrite(SHCP_pin,LOW);
    digitalWrite(STCP_pin,LOW);
    digitalWrite(DS_pin,registers[i]);
    //delay(1);
    digitalWrite(STCP_pin,HIGH);
    digitalWrite(SHCP_pin,HIGH);
  }
  
  digitalWrite(SHCP_pin,LOW);
  digitalWrite(STCP_pin,LOW);
  digitalWrite(STCP_pin,HIGH);
  digitalWrite(SHCP_pin,HIGH);
  
}

void loop()
{
  setreg((int [16]){1,1,1,1,1,0,0,0,0,1,1,1,0,0,1,0});
  writereg();
  delay(1000);
  /*
  int a[8];
  for (int i=0;i<8;i++)
    a[i]=(i%2==0);
  setreg(a);
  writereg();
  delay(500);
  for (int i=0;i<8;i++)
    a[i]=(i%2!=0);
  setreg(a);
  writereg();
  delay(500);
  */
}

void setreg(int a[])
{
  for (int i=0;i<16;i++)
    registers[i]=a[i];
}
