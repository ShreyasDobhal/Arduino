// IARC

int a,b,c,d,e,f; // IR Sensor Array 
int ml1=11,ml2=10,mr1=8,mr2=9;  // Motors
int snrIn[3]={2,4,6},snrOut[3]={3,5,7}; // ultrasonic sensors
// 0 for Left 1 for Right and 2 for Forward
int blkcnt=0; // Count for Blocks
long St1=0,St2=0; // Time variables for Sonar
int LEDseg=12; // Seven segment display
boolean over=false; // Path comletion variable
int flag=0; // Variable for denotion of part of the path
int thres=500; // Threshold for the sensors
int val=0; // debugging variable

void setup()
{ 
  Serial.begin(9600);
  pinMode(ml1,OUTPUT);
  pinMode(mr1,OUTPUT);
  pinMode(ml2,OUTPUT);
  pinMode(mr2,OUTPUT);
  
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}//setup()

void loop() 
{
  St1=millis(); // initial time
  blkcnt=0;
  // Line Following
  startLine();
  // Sonar Detection
  //startSonar();
  
  // debugging
  // debug(1); // for analogRead
  // debug(2); // for digitalRead
  // debug(3); // for sonarRead
  // debug(4); // for directino commands
  // debug(5); // for controlling motors
}//loop()

void startLine()
{ 
  digitalWrite(13,LOW);
  int t;
  while(1)
  {
    read_sensor();
    if(!over)
    {
      move_forward();
      t=node_type();
      switch(t)
      {
        case 0:
          //over=true;
          // Over complete black area
          if (is_end())
          {
            over=true;
            digitalWrite(ml1,LOW);
            digitalWrite(ml2,LOW);
            digitalWrite(mr1,LOW);
            digitalWrite(mr2,LOW);
            return;
          }//if block
          break;
        case 1:
          move_left();
          break;
        case 2:
          move_straight(); // Not called
          break;
        case 3:
          move_right();
          break;
        case 4:
          // Change of Path
          digitalWrite(ml1,HIGH);
          digitalWrite(ml2,HIGH);
          digitalWrite(mr1,HIGH);
          digitalWrite(mr2,HIGH);
          if (flag==0)
          {
            // Activate Sonar
            flag=1;
          }//if block
          return;
      }//switch block
    }//if block
  }//while loop
}//startLine()

void startSonar()
{
  digitalWrite(13,HIGH);
  int l=sonarLeft();
  int r=sonarRight();
  int forw=0,dist=0;
  int dir=0; // Direction
  if (l<r&&l!=0)
  {
    // wall is on the left hand side
    dir=0;
    dist=l;
  }//if block
  else if (r<l&&r!=0)
  {
    // wall is on the right hand side
    dir=1;
    dist=r;
  }//if block
  while (1)
  {
    forw=sonarForward();
    if (forw>dist)
    {
      // Wall in front is not near
      // so moving parallel to the side wall
      int x=sonar(dir);
      if (dist-x>10)
      {
        // Distance between the wall is decreasing
        // So moving away from the wall
        if (dir==0) // Wall on left, move towards right
        {
          digitalWrite(ml1,HIGH);
          digitalWrite(ml2,LOW);
          digitalWrite(mr1,LOW);
          digitalWrite(mr2,LOW);
          delay(100);
        }//if block
        else if (dir==1) // Wall on right, move towards left
        {
          digitalWrite(ml1,LOW);
          digitalWrite(ml2,LOW);
          digitalWrite(mr1,HIGH);
          digitalWrite(mr2,LOW);
          delay(100);
        }//else if block
      }//if block
      
      else if (x-dist>10)
      {
        // Distance between the wall is increasing
        // So moving towards from the wall
        if (dir==0) // Wall on left, move towards left
        {
          digitalWrite(ml1,LOW);
          digitalWrite(ml2,LOW);
          digitalWrite(mr1,HIGH);
          digitalWrite(mr2,LOW);
          delay(100);
        }//if block
        else if (dir==1) // Wall on right, move towards right
        {
          digitalWrite(ml1,HIGH);
          digitalWrite(ml2,LOW);
          digitalWrite(mr1,LOW);
          digitalWrite(mr2,LOW);
          delay(100);
        }//else if block
      }//else if block
      
      else
      {
        // Distance between the wall is fairly constant
        // so moving forward
        digitalWrite(ml1,HIGH);
        digitalWrite(ml2,LOW);
        digitalWrite(mr1,HIGH);
        digitalWrite(mr2,LOW);
        delay(100);
      }//else block
    }//if block
    
    else
    {
      // Wall in front is near now
      // so turning to more clear side
      if (dir==0) // if wall on left side turn right
      {
        digitalWrite(ml1,HIGH);
        digitalWrite(ml2,LOW);
        digitalWrite(mr1,LOW);
        digitalWrite(mr2,HIGH);
        delay(250);
      }//if block
      else if (dir==1) // if wall on right side turn left
      {
        digitalWrite(ml1,LOW);
        digitalWrite(ml2,HIGH);
        digitalWrite(mr1,HIGH);
        digitalWrite(mr2,LOW);
        delay(250);
      }//if block
    }//else block
  }//while loop
}//startSonar()

void move_left()
{
  read_sensor();
  digitalWrite(ml1,HIGH);
  digitalWrite(ml2,HIGH);
  digitalWrite(mr1,HIGH);
  digitalWrite(mr2,LOW);
  delay(250);
  read_sensor();
  while(c&&d)
  {
    digitalWrite(ml1,HIGH);
    digitalWrite(ml2,HIGH);
    digitalWrite(mr1,HIGH);
    digitalWrite(mr2,LOW);
    delay(50);
    read_sensor();
  }//while loop
}//move_left()

void move_right()
{
  read_sensor();
  digitalWrite(ml1,HIGH);
  digitalWrite(ml2,LOW);
  digitalWrite(mr1,HIGH);
  digitalWrite(mr2,HIGH);
  delay(250);
  read_sensor();
  while(c&&d)
  {
    digitalWrite(ml1,HIGH);
    digitalWrite(ml2,LOW);
    digitalWrite(mr1,HIGH);
    digitalWrite(mr2,HIGH);
    delay(50);
    read_sensor();
  }//while loop  
}//move_right

void move_straight()
{
  digitalWrite(ml1,HIGH);
  digitalWrite(ml2,LOW);
  digitalWrite(mr1,HIGH);
  digitalWrite(mr2,LOW);
  delay(10);
}//move_straight()

void move_forward()
{
  while(1)
  {
    read_sensor();
    if(a&&b&&(!c||!d)&&e&&f)
    {
      digitalWrite(ml1,HIGH);
      digitalWrite(ml2,LOW);
      digitalWrite(mr1,HIGH);
      digitalWrite(mr2,LOW); 
    }//if block
    else if((a&&b&&!d&&!e&&f)||(b&&c&&d&&!e&&a))
    {
      digitalWrite(mr1,LOW);
      digitalWrite(mr2,LOW);
      digitalWrite(ml1,HIGH);
      digitalWrite(ml2,LOW); 
    }//else if block
    else  if((a&&!b&&!c&&e&&f)||(!b&&c&&d&&e&&&f))
    {
      digitalWrite(mr1,HIGH);
      digitalWrite(mr2,LOW);
      digitalWrite(ml1,LOW);
      digitalWrite(ml2,LOW); 
    }//else if block 
    else
      break;
  }//while loop
}//move_forward()

int node_type()
{
  read_sensor();
  if (!a&&!b&&!c&&!d&&!e&&!f)
    return 0; // End
  if (!a||!b)
    return 1; // Left
  if(!e||!f)
    return 3; // Right
  if(a&&b&&c&&d&&e&&f) // end point
    return 4; // Change of path
}//node_type

int is_end()
{
  read_sensor();
  long t1=millis();
  long t2=t1;
  while (!a&&!b&&!c&&!d&&!e&&!f)
  {
    if ((t2-t1)>=500)
      return 0;   
    t2=millis();
  }//while loop
  return 1;
}//is_end()

void read_sensor()
{
  a=analogRead(A0);
  b=analogRead(A1);
  c=analogRead(A2);
  d=analogRead(A3);
  e=analogRead(A4);
  f=analogRead(A5);
  // Conveting analog to digital value
  a=(a>thres)?(1):(0);
  b=(b>thres)?(1):(0);
  c=(c>thres)?(1):(0);
  d=(d>thres)?(1):(0);
  e=(e>thres)?(1):(0);
  f=(f>thres)?(1):(0);
    
  delay(2);
}//read_sensor()

long sonarLeft()
{
  return sonar(0);
}//sonarLeft()
long sonarRight()
{
  return sonar(1);
}//sonarRight()

long sonarForward()
{
  return sonar(2);
}//sonarForward()

long sonar(int x)
{
  long duration, cm;
  pinMode(snrOut[x], OUTPUT);
  digitalWrite(snrOut[x], LOW);
  delayMicroseconds(2);
  digitalWrite(snrOut[x], HIGH);
  delayMicroseconds(5);
  digitalWrite(snrOut[x], LOW);
  pinMode(snrIn[x], INPUT);
  duration = pulseIn(snrIn[x], HIGH);
  cm = (duration / 29 / 2);
  return cm;
}//sonar()

void Blink()
{
  for (int cnt=0;cnt<blkcnt;cnt++)
  {
    digitalWrite(LEDseg,HIGH);
    delay(10);
    digitalWrite(LEDseg,LOW);
    delay(10);
  }//for loop
}//Blink

// Remove Later
void debug(int x)
{
  int pins[6]={A0,A1,A2,A3,A4,A5};
  switch (x)
  {
    case 1:
      // Printing sensor values
      
      for (int i=0;i<6;i++)
      {
        Serial.print(analogRead(pins[i]));
        Serial.print(" , ");
      }//for loop
      Serial.println();
      delay(100);
      break;
    case 2:
      // Printing the digital Sensor Values
      read_sensor();
      Serial.print(a);
      Serial.print(" , ");
      Serial.print(b);
      Serial.print(" , ");
      Serial.print(c);
      Serial.print(" , ");
      Serial.print(d);
      Serial.print(" , ");
      Serial.print(e);
      Serial.print(" , ");
      Serial.println(f);
      delay(100);
      break;
    case 3:
      // Printing sonar values
      Serial.print(sonarLeft());
      Serial.print(" , ");
      Serial.print(sonarForward());
      Serial.print(" , ");
      Serial.println(sonarRight());
      delay(100);
      break;
    case 4:
      // Printing the path command
      val=node_type();
      switch (val)
      {
        case 0:
          Serial.println("End");
          break;
        case 1:
          Serial.println("Left");
          break;
        case 3:
          Serial.println("Right");
          break;
        case 4:
          Serial.println("Path change");
          break;
      }//switch block
      break;
    case 5:
      // Controlling motors
      if (Serial.available()>0)
      {
        val=Serial.read();
      }//if block
      switch (val)
      {
        case 'w':
          digitalWrite(ml1,HIGH);
          digitalWrite(ml2,LOW);
          digitalWrite(mr1,HIGH);
          digitalWrite(mr2,LOW);
          break;
        case 'a':
          digitalWrite(ml1,LOW);
          digitalWrite(ml2,LOW);
          digitalWrite(mr1,HIGH);
          digitalWrite(mr2,LOW);
          break;
        case 'd':
          digitalWrite(ml1,HIGH);
          digitalWrite(ml2,LOW);
          digitalWrite(mr1,LOW);
          digitalWrite(mr2,LOW);
          break;
        case 's':
          digitalWrite(ml1,LOW);
          digitalWrite(ml2,HIGH);
          digitalWrite(mr1,LOW);
          digitalWrite(mr2,HIGH);
          break;
        default:
          digitalWrite(ml1,LOW);
          digitalWrite(ml2,LOW);
          digitalWrite(mr1,LOW);
          digitalWrite(mr2,LOW);
          break;
      }//switch block
      break;
  }//switch block
}//debug()
