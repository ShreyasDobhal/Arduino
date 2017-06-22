#define MaxSpeed 150
#define MaxTurnSpeed 100

int led=13;
int ml1=2,ml2=3,mr1=5,mr2=4;
int val=0;
int dump=0;

void setup()
{
  pinMode(led,OUTPUT);
  pinMode(ml1,OUTPUT);
  pinMode(ml2,OUTPUT);
  pinMode(mr1,OUTPUT);
  pinMode(mr2,OUTPUT);
  
  digitalWrite(led,LOW);
  digitalWrite(ml1,LOW);
  digitalWrite(ml2,LOW);
  digitalWrite(mr1,LOW);
  digitalWrite(mr2,LOW);
  
  Serial.begin(9600);
}//setup()

void loop()
{
  if (Serial.available()>0)
  {
    val=Serial.read();
    dump=Serial.read();
  }//if block
  
  if(val=='0'||dump=='0') // stop
  {
    brake();
    val=0;
    dump=0;
  }
  else if (val=='1'||dump=='1') // forward
  {
    forward();
    val=0;
    dump=0;
  }
  else if (val=='2'||dump=='2') // right
  {
    right();
    val=0;
    dump=0;
  }
  else if (val=='3'||dump=='3') // left
  {
    left();
    val=0;
    dump=0;
  }
  else if (val=='4'||dump=='4') // reverse
  {
    reverse();
    val=0;
    dump=0;
  }
  
  delay(50);
}//loop()
// analog works on ml2 and mr1
void forward()
{
  digitalWrite(ml1,HIGH);
  analogWrite(ml2,255-MaxSpeed);
  analogWrite(mr1,MaxSpeed);
  digitalWrite(mr2,LOW);
}

void reverse()
{
  digitalWrite(ml1,LOW);
  analogWrite(ml2,MaxSpeed);
  analogWrite(mr1,255-MaxSpeed);
  digitalWrite(mr2,HIGH);
}

void left()
{
  digitalWrite(ml1,LOW);
  analogWrite(ml2,MaxTurnSpeed);
  analogWrite(mr1,MaxTurnSpeed);
  digitalWrite(mr2,LOW);
}

void right()
{
  digitalWrite(ml1,HIGH);
  analogWrite(ml2,255-MaxTurnSpeed);
  analogWrite(mr1,255-MaxTurnSpeed);
  digitalWrite(mr2,HIGH);
}

void brake()
{
  digitalWrite(ml1,LOW);
  analogWrite(ml2,0);
  analogWrite(mr1,0);
  digitalWrite(mr2,LOW);
}
