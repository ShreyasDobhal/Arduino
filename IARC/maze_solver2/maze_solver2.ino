int a,b,c,f,d,e,g,ml1=6,ml2=7,mr1=4,mr2=5;  //sensors from left to right
int p, top=-1,i,j,k;          //p=front sensor,  top=top of stack for turns
char ch[33]={0},s[100];
int snr1=A0,snr2=A1;
int blkcnt=0;
long st1=0,st2=0;
int led=13; /////////////////// 13
/*  1-only left,left and straight, left and right,left and straight and right
2- straight and right

3- only right 

4- dead end
*/

void setup() {
  // put your setup code here, to run once:
pinMode(ml1,OUTPUT);
pinMode(mr1,OUTPUT);
pinMode(ml2,OUTPUT);
pinMode(mr2,OUTPUT);
pinMode(led,OUTPUT);
}

void loop() 
{
        st1=millis();
        blkcnt=0;
       start();   
       shortest();
       reverse();
       move_shortest();
      
}

void start()
{ 
   int t;
while(1)
  { read_sensor();

st2=millis();
if ((check_block()==1)&&(st2-st1>=2000))
{
  st1=st2;
  blkcnt++;
}
  if (check_block()==2) 
     {
        digitalWrite(ml1,LOW);
        digitalWrite(ml2,LOW);
        digitalWrite(mr1,LOW);
        digitalWrite(mr2,LOW);
        Blink();
        
        //break;
        //return;
     }
    if(is_end())
     {  move_forward();
       t=node_type();
        switch(t)
       { case 1:{   move_left();
                 s[top++]='L';
             }
                 break;
         case 2:{    move_straight();
                s[top++]='S';
           }
                break;
          case 3:{    move_right();
                s[top++]='R';
           }
                break;
          case 4:{    move_back();
                   s[top++]='B';
           }
                break;
    
        }
     }
     else 
     {
        digitalWrite(ml1,LOW);
        digitalWrite(ml2,LOW);
        digitalWrite(mr1,LOW);
        digitalWrite(mr2,LOW);
        //Blink();
        
        //break;
        return;
     }
     }
  }


void shortest()
{  int i,j,k;
for(j=0;j<top-2;j++)
for(i=0;i<top-2;i++)
{   
    if(s[i+1]=='B')
    {   if(s[i]=='L'&&s[i+2]=='R')
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
    }  
    
    for(j=i;j<top-2;j++)
    {  s[j]=s[j+2];
    }
    top-=2;
     
}


}


void reverse()
{    for(i=0;i<top;i++)
     {  if(s[i]=='L')
            s[i]='R';
            else if(s[i]=='R')
               s[i]='L';
     }

}




void move_shortest()
{   j=top;
    
     while(1)
     {  move_forward();
     //read_sensor();
        if(is_node())
         {  if(s[j--]=='L')
                 move_left();
                 else if(s[j--]=='S')
                           move_straight();
                           else if(s[j--]=='R')
                               move_right();
                               
         }
     }  
}

int is_node()
{    read_sensor();
      if((!a&&!b)||(!e&&!f))
         return 1;
         else 
          return 0;

}

void move_left()
{  read_sensor();
 //digitalWrite(ml1,HIGH);
 //digitalWrite(ml2,LOW);
 //digitalWrite(mr1,HIGH);
 //digitalWrite(mr2,LOW);
 //delay(150);
 //digitalWrite(ml1,HIGH);
 //digitalWrite(ml2,HIGH);
 //digitalWrite(mr1,HIGH);
 //digitalWrite(mr2,HIGH);
 //delay(200);
 digitalWrite(ml1,HIGH);
 digitalWrite(ml2,HIGH);
 digitalWrite(mr1,HIGH);
 digitalWrite(mr2,LOW);
 delay(250);
 read_sensor();
 while(c||d)
 {      digitalWrite(ml1,HIGH);
        digitalWrite(ml2,HIGH);
        digitalWrite(mr1,HIGH);
        digitalWrite(mr2,LOW);
        delay(50);
        read_sensor();
        
 }
}

void move_right()
{read_sensor();
 //digitalWrite(ml1,HIGH);
 //digitalWrite(ml2,LOW);
 //digitalWrite(mr1,HIGH);
 //digitalWrite(mr2,LOW);
 //delay(150);
 //digitalWrite(ml1,HIGH);
 //digitalWrite(ml2,HIGH);
 //digitalWrite(mr1,HIGH);
 //digitalWrite(mr2,HIGH);
 //delay(200);
 digitalWrite(ml1,HIGH);
 digitalWrite(ml2,LOW);
 digitalWrite(mr1,HIGH);
 digitalWrite(mr2,HIGH);
 delay(250);
 read_sensor();
 while(c||d)
 {
        digitalWrite(ml1,HIGH);
        digitalWrite(ml2,LOW);
        digitalWrite(mr1,HIGH);
        digitalWrite(mr2,HIGH);
        delay(50);
        read_sensor();
 }  
}

void move_straight()
{     digitalWrite(ml1,HIGH);
      digitalWrite(ml2,LOW);
      digitalWrite(mr1,HIGH);
      digitalWrite(mr2,LOW);
      delay(10);
}

void move_back()
{   read_sensor();
    digitalWrite(mr1,HIGH);
    digitalWrite(mr2,LOW);
    digitalWrite(ml1,HIGH);
    digitalWrite(ml2,LOW);
   delay(250);
   //read_sensor();
   digitalWrite(ml1,HIGH);
   digitalWrite(ml2,LOW);
   digitalWrite(mr1,LOW);
   digitalWrite(mr2,HIGH);
   delay(125);
   read_sensor();
 while(!(a&&b&&!c&&!d&&e&&f))
  {   
       digitalWrite(mr1,HIGH);
      digitalWrite(mr2,LOW); 
      digitalWrite(ml1,LOW);  
      digitalWrite(ml2,HIGH);
      delay(10);
      read_sensor();
  }
}

void move_forward()
{   while(1)
    {read_sensor();
    if( a&&b&&!c&&!d&&e&&f)
    {
      digitalWrite(ml1,HIGH);
      digitalWrite(ml2,LOW);
      digitalWrite(mr1,HIGH);
      digitalWrite(mr2,LOW); 
     }
     else  if((a&&b&&(c||!c)&&!d&&!e&&f)||((f||!f)&&b&&c&&d&&!e&&&a))
    {
      digitalWrite(mr1,LOW);
      digitalWrite(mr2,LOW);
      digitalWrite(ml1,HIGH);
      digitalWrite(ml2,LOW); 
     }
     else  if((a&&!b&&!c&&(d||!d)&&e&&f)||((a||!a)&&!b&&c&&d&&e&&&f))
    {
      digitalWrite(mr1,HIGH);
      digitalWrite(mr2,LOW);
      digitalWrite(ml1,LOW);
      digitalWrite(ml2,LOW); 
     } 
     else 
        break;
  }
}

void read_sensor()
{       //lcd_clear();
        a=digitalRead(8);
        b=digitalRead(9);
        c=digitalRead(10);
        d=digitalRead(11);
        e=digitalRead(12); //////////////////// 12
        f=digitalRead(3);
        g=digitalRead(2);
        //(ch,"%d %d <%d> %d %d |%d|",a,b,c,d,e,p);
        //lcd_puts(ch);  
        delay(5);
       
      
}

int node_type()
{    read_sensor();
        //if((!a&&!b&&!c&&!d&&!e&&!f)||(!a&&!b&&!c&&!d&&e&&f)||(!a&&!b&&!c&&d&&e&&f)||(!a&&!b&&!c&&!d&&!e&&f))   //left and right only || left and straight  || only left || left and straight and right
        if (!a||!b)
         return 1;
         else  if((a&&b&&(!c||!d))&&!g)      //straight and right
         return 2;
         else   if((a&&b&&(!c||!d)&&(!e||!f)&&g))   //only right 
         return 3;
         else  if(a&&b&&c&&d&&e&&f&&g) //dead end
         return 4;
}

int is_end()
{ read_sensor();
long t1=millis();
long t2=t1;
while (!a&&!b&&!c&&!d&&!e&&!f)
{
  if ((t2-t1)>=1500)
    return 0;   
  t2=millis();
}//while loop
return 1;
}
int check_block()
{
  int sa=analogRead(snr1);
  int sb=analogRead(snr2);
  if (sa>500&&sb>500)
    return 2;
  if (sa>500||sb>500)
    return 1;
  return 0;
}
void Blink()
{
  for (int cnt=0;cnt<blkcnt;cnt++)
  {
    digitalWrite(led,HIGH);
    delay(250);
    digitalWrite(led,LOW);
    delay(250);
  }
}
