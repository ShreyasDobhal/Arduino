int a,b,c,d,e,f; // IR Sensor Array 
int ml1=6,ml2=7,mr1=4,mr2=5;  // Motors
int snr1=2,snr2=3; // ultrasonic sensors default pin7 
int p,top=-1,i,j,k;          //p=front sensor,  top=top of stack for turns
char ch[33]={0},s[100];
int blkcnt=0; // Count for Blocks
long St1=0,St2=0; // Time variables for Sonar
int led=1; ///////////////////////////////////////////////////////////////////
/*
1-only left,left and straight, left and right,left and straight and right
2- straight and right
3- only right 
4- dead end
*/

void setup()
{ 
  Serial.begin(9600);
  pinMode(ml1,OUTPUT);
  pinMode(mr1,OUTPUT);
  pinMode(ml2,OUTPUT);
  pinMode(mr2,OUTPUT);
}//setup()

void loop() 
{
  St1=millis(); // initial time
  blkcnt=0;
  start();
  shortest();
  reverse();
  move_shortest();    
}//loop()

void start()
{ 
  int t;
  while(1)
  {
    read_sensor();
    
    if ((!a&&!b))  // Move left
    {
      digitalWrite(ml1,LOW);
      digitalWrite(ml2,LOW);
      digitalWrite(mr1,HIGH);
      digitalWrite(mr2,LOW);
      delay(250);
      read_sensor();
      while (c&&d)
      {
        digitalWrite(ml1,LOW);
        digitalWrite(ml2,LOW);
        digitalWrite(mr1,HIGH);
        digitalWrite(mr2,LOW);
        delay(50);
        read_sensor();
      }//while loop
      s[top++]='L';
    }//if block
    else if (!c&&!d&&!e&&!f) // Move Straight
    {
      while (!(a&&b&&!c&&!d&&e&&f))
      {
        digitalWrite(ml1,HIGH);
        digitalWrite(ml2,LOW);
        digitalWrite(mr1,HIGH);
        digitalWrite(mr2,LOW);
        //read_sensor();
        delay(150);
        read_sensor();
      }
      s[top++]='S';
    }//if block
    else if (a&&b&&c&&d&&!e&&!f) // Move Right
    {
      digitalWrite(ml1,HIGH);
      digitalWrite(ml2,LOW);
      digitalWrite(mr1,LOW);
      digitalWrite(mr2,LOW);
      delay(150);
      read_sensor();
      while (c&&d)
      {
        digitalWrite(ml1,HIGH);
        digitalWrite(ml2,LOW);
        digitalWrite(mr1,LOW);
        digitalWrite(mr2,LOW);
        //read_sensor();
        delay(50);
        read_sensor();
      }//while loop
      s[top++]='R';
    }//if block
    
    else if (a&&b&&c&&d&&e&&f) // reverse
    {
      digitalWrite(ml1,HIGH);
      digitalWrite(ml2,LOW);
      digitalWrite(mr1,HIGH);
      digitalWrite(mr2,LOW);
      delay(125);
      read_sensor();
      while (!(a&&b&&!c&&!d&&e&&f))
      {
        digitalWrite(ml1,LOW);
        digitalWrite(ml2,HIGH);
        digitalWrite(mr1,HIGH);
        digitalWrite(mr2,LOW);
        delay(10);
        read_sensor();
      }//while loop
      s[top++]='B';
    }//else if block
    
    else if (a&&b&&!c&&!d&&e&&f)
    {
      digitalWrite(ml1,HIGH);
      digitalWrite(ml2,LOW);
      digitalWrite(mr1,HIGH);
      digitalWrite(mr2,LOW);
    }//else if block
    
    /*
    // Checking for the block
    St2=millis();
    if (check_block()&&(St2-St1>=5000))
    {
      St1=St2;
      blkcnt++;
    }//if block
    */
    /*
    if(is_end()) // 0 if end else 1
    {
      move_forward();
      t=node_type();
      switch(t)
      {
        case 1:
          Serial.println("Left");
          move_left();
          s[top++]='L';
          break;
        case 2:
          Serial.println("straight");
          move_straight();
          s[top++]='S';
          break;
        case 3:
          Serial.println("Right");
          move_right();
          s[top++]='R';
          break;
        case 4:
          Serial.println("Back");
          move_back();
          s[top++]='B';
          break;
      }//switch block
    }//if block
    else
    {
      // Path ended
      // Blinking the LED
      Blink();
      break;
    }//else block
    */
  }//while loop
}//start()

void shortest()
{
  int i,j,k;
  for(j=0;j<top-2;j++)
    for(i=0;i<top-2;i++)
    {   
      if(s[i+1]=='B')
      {
        if(s[i]=='L'&&s[i+2]=='R')
          s[i]='B';
        else if(s[i]=='L'&&s[i+2]=='S')
          s[i]='R';
        else if(s[i]=='R'&&s[i+2]=='L')
          s[i]='B';
        else if(s[i]=='S'&&s[i+2]=='L')
          s[i]='R';
        else if(s[i]=='S'&&s[i+2]=='S')
          s[i]='B';
        else if(s[i]=='L'&&s[i+2]=='L')
          s[i]='S';
      }//if block  
      for(j=i;j<top-2;j++)
      {
        s[j]=s[j+2];
      }//for loop
      top-=2;
    }//for loop
}//shortest()

void reverse()
{
  for(i=0;i<top;i++)
  {
    if(s[i]=='L')
      s[i]='R';
    else if(s[i]=='R')
      s[i]='L';
  }//for loop
}//reverse()

void move_shortest()
{
  j=top;
  while(1)
  {
    move_forward();
    if(is_node())
    {
      if(s[j--]=='L')
        move_left();
      else if(s[j--]=='S')
        move_straight();
      else if(s[j--]=='R')
        move_right();
    }//if block
  }//while loop  
}//move_shortest()

int is_node()
{
  read_sensor();
  if(b||d)
    return 1;
  else 
    return 0;
}//is_node()




void move_left()
{
  read_sensor();
  digitalWrite(ml1,HIGH);
  digitalWrite(ml2,LOW);
  digitalWrite(mr1,HIGH);
  digitalWrite(mr2,LOW);
  delay(150);
  
  digitalWrite(ml1,LOW);
  digitalWrite(ml2,LOW);
  digitalWrite(mr1,LOW);
  digitalWrite(mr2,LOW);
  delay(40);
  
  digitalWrite(ml1,LOW);
  digitalWrite(ml2,LOW);
  digitalWrite(mr1,HIGH);
  digitalWrite(mr2,LOW);
  delay(125);
  
  while(c)
  {
    read_sensor();
    digitalWrite(ml1,LOW);
    digitalWrite(ml2,LOW);
    digitalWrite(mr1,HIGH);
    digitalWrite(mr2,LOW);
  }//while loop  
}//move_left()

void move_right()
{
  read_sensor();
  digitalWrite(ml1,HIGH);
  digitalWrite(ml2,LOW);
  digitalWrite(mr1,HIGH);
  digitalWrite(mr2,LOW);
  delay(150);
  
  digitalWrite(ml1,LOW);
  digitalWrite(ml2,LOW);
  digitalWrite(mr1,LOW);
  digitalWrite(mr2,LOW);
  delay(40);
  
  digitalWrite(ml1,HIGH);
  digitalWrite(ml2,LOW);
  digitalWrite(mr1,LOW);
  digitalWrite(mr2,LOW);
  delay(125);
  
  while(c)
  {
    read_sensor();
    digitalWrite(ml1,HIGH);
    digitalWrite(ml2,LOW);
    digitalWrite(mr1,LOW);
    digitalWrite(mr2,LOW);
  }//while loop  
}//move_right()

void move_straight()
{
  digitalWrite(ml1,HIGH);
  digitalWrite(ml2,LOW);
  digitalWrite(mr1,HIGH);
  digitalWrite(mr2,LOW);
  delay(100);
}//move_straight()

void move_back()
{
  read_sensor();
  digitalWrite(mr1,HIGH);
  digitalWrite(mr2,LOW);
  digitalWrite(ml1,HIGH);
  digitalWrite(ml2,LOW);
  delay(150);
  
  digitalWrite(mr1,HIGH);
  digitalWrite(mr2,LOW); 
  digitalWrite(ml1,LOW);  
  digitalWrite(ml2,HIGH);
  delay(250);
  
  while(c||d)
  {
    read_sensor();
    digitalWrite(mr1,HIGH);
    digitalWrite(mr2,LOW); 
    digitalWrite(ml1,LOW);  
    digitalWrite(ml2,HIGH);
  }//while loop
}//move_back();

void move_forward()
{
  while(1)
  {
    read_sensor();
    if( a&&b&&!c&&!d&&e&&f)
    {
      digitalWrite(ml1,HIGH);
      digitalWrite(ml2,LOW);
      digitalWrite(mr1,HIGH);
      digitalWrite(mr2,LOW); 
    }//if block
    else  if(a&&!b&&!c&&d&&e&&f)
    {
      digitalWrite(ml1,LOW);
      digitalWrite(ml2,LOW);
      digitalWrite(mr1,HIGH);
      digitalWrite(mr2,LOW); 
    }//else if block
    else  if(a&&b&&c&&!d&&!e&&f)
    {
      digitalWrite(ml1,HIGH);
      digitalWrite(ml2,LOW);
      digitalWrite(mr1,LOW);
      digitalWrite(mr2,LOW); 
    }//else if block 
    else 
       break;
  }//while loop
}//move_forward()

void read_sensor()
{
  a=digitalRead(8);
  b=digitalRead(9);
  c=digitalRead(10);
  d=digitalRead(11);
  e=digitalRead(12);
  f=digitalRead(13);
  delay(5);
}//read_sensor()

int node_type()
{
  read_sensor();
  if((!a&&!b&&!c&&!d&&!e&&!f)||(!a&&!b&&!c&&!d&&e&&f)||(!a&&!b&&!c&&d&&e&&f))   //left and right only || left and straight  || only left || left and straight and right
    return 1;
  else  if((a&&b&&!c&&!d&&!e&&!f))      //straight and right
    return 2;
  else   if(a&&b&&!e&&!f)   //only right 
    return 3;
  else  if(a&&b&&c&&d&&e&&f) //dead end 
    return 4;      
}//node_type()

int is_end()
{
  read_sensor();
  long t1=millis();
  long t2=t1;
  while(!a&&!b&&!c&&!d&&!e&&!f)
  {
    if ((t2-t1)>=500)
      return 0; // end point
    t2=millis();
  }//while loop
  return 1;
}//is_end()

int check_block()
{
  long duration,cm;
  
  // Checking in one direction
  pinMode(snr1, OUTPUT);
  digitalWrite(snr1, LOW);
  delayMicroseconds(2);
  digitalWrite(snr1, HIGH);
  delayMicroseconds(5);
  digitalWrite(snr1, LOW);
  
  pinMode(snr1, INPUT);
  duration = pulseIn(snr1, HIGH);
  cm = duration/29/2;
  
  if (cm<30)
    return 1; // Block detected
  
  // Checking in the other direction
  pinMode(snr2, OUTPUT);
  digitalWrite(snr2, LOW);
  delayMicroseconds(2);
  digitalWrite(snr2, HIGH);
  delayMicroseconds(5);
  digitalWrite(snr2, LOW);
  
  pinMode(snr2, INPUT);
  duration = pulseIn(snr2, HIGH);
  cm = duration/29/2;
  
  if (cm<30)
    return 1; // Block detected
  
  return 0; // Block not detected
  
  delay(2);
}//check_block()

void Blink()
{
  for (int cnt=0;cnt<blkcnt;cnt++)
  {
    digitalWrite(led,HIGH);
    delay(250);
    digitalWrite(led,LOW);
    delay(250);
  }//for loop
}//Blink
