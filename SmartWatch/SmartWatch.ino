/*
*
*  ----------------------  Smart Watch -------------------------
*
*  Compatibile with Arduino Nano V3.0
*  and Arduino IDE 1.0.5-r2
*
*  Sensors : -
*  1) A0 - Potentiometer
*  2) A1 - LDR                   (Optional)
*  3) 02 - Capacitive Sensor     (OPtional)
*  4) 03 - Physical Button
*  5) 04 - Touch Reciever        (Optional)
*  6) Rx - Bluetooth Receiver    (Optional)
*  7) Tx - Bluetooth Transmitter (Optional)
*  8) 13 - Bluetooth Power       (Optional)
*/

#include <LCD5110_Graph.h>
#include <CapacitiveSensor.h>

LCD5110 lcd(8,9,10,11,12);
extern uint8_t SmallFont[];
CapacitiveSensor cs_4_2=CapacitiveSensor(4,2);
int led=5; // Backlight on pin5 (PWM)
int BT=13; // Bluetooth Module (Activate) //////////////////////
int btn=3; // Push Button on pin 3 (Digital)
int h=0;  // Hours
int hr=0; // Substitute of Hours
int m=0;  // Minutes
int s=0;  // Seconds
int ms=0; // Milli Seconds
int val=0;
int lastval=0;
int mar=0;// Marker/Pointer for selecting options
int option=0;
int scr=0; // Current Screen
int temp=0; // Temporary Variable for local usage
int score=0;
int bt=0; // Data send via Bluetooth
// 0 - Initialized
// 1 - Turn on the Light
// 2 - Turn off the light
// 3 - Message Recieved
// 4 - Missed Call
// 5 - Back
byte a=100;  // Coordinates of enemy
byte b=24;  // Coordinates of enemy
byte x=15; // Coordinates of player
byte y=19; // Coordinates of player
byte spd=1;
boolean flag=true;
boolean once=true; // Resetting the obstacle in the game
boolean ana=true; // Analog otherwise Digital
boolean slp=false; // Sleep Mode
boolean tou=false; // Touch Mode
// 0 - Time
// 1 - Main Menu
// 2 - Back Light
// 3 - Set Brightness
// 4 - Game (Flappy)
// 5 - Settings
// 6 - Set Time
// 7 - Profile
// 8 - Connected

void setup()
{
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
  lcd.InitLCD();
  lcd.setContrast(70);
  lcd.setFont(SmallFont);
  pinMode(led,OUTPUT);
  pinMode(BT,OUTPUT);
  pinMode(btn,INPUT);
  digitalWrite(led,HIGH);
  digitalWrite(BT,LOW);
  Serial.begin(9600);
  lcd.clrScr();
  lcd.update();
}//setup()
void loop()
{
  
  // -------- Serial Commmunication ----------
  if (Serial.available()>0)
  {
    bt=Serial.read();
    if (bt=='0')
      scr=8; // Goto Connected Screen
    lcd.disableSleep();
    lcd.clrScr();
    lcd.update();
  }//if block
  
  // ---------- Button Pressed ------------
  val=digitalRead(btn);
  
  if (scr==0) // Presently in Time Screen
  {
    if (tou&&val==1)
    {
      lcd.disableSleep();
      lcd.update();
      onbl();
      lastval=val;
      if (analogRead(A0)>1020)
      {
        scr=0;
        tou=false;
      }//if block
    }//if block
    else if (tou&&val==0)
    {
      lcd.enableSleep();
      offbl();
      lastval=val;
    }//else if block
  }//if block
  
  if (val==1&&val!=lastval) // Button Pressed (Clicked)
  {
    lcd.clrScr();
    lcd.update();
    if (scr==0) // Presently in the Time Screen
    {
      if (slp) // Watch is asleep (wake it up!)
      {
        slp=false;
        lcd.disableSleep();
        lcd.update();
        onbl();
      }//if block
      else 
        scr=1;  // Goto Main Menu
    }//if block
    else if (scr==1) // Presently in Main Menu
    {
      if (mar==10)
        scr=2;  // Goto BackLight
      if (mar==20)
        scr=5; // Goto Set Time / Settings
      if (mar==30)
        scr=4;// Goto Extras / Games
      if (mar==40)
        scr=0; // Goto Time Screen
    }//else if
    else if (scr==2) // Presently in BackLight
    {
      if (mar==10)
      {
        if (digitalRead(led)==HIGH)
          offbl(); // Turn off the back light
        else
          onbl(); // Turn on the back light
      }//if block
      else if (mar==20)
        scr=3; // Goto Set Brightness
      else if (mar==30)
        analogWrite(led,75);
      else if (mar==40)
        scr=1; // Goto Main Menu
    }//else if
    else if (scr==3) // Presently in Set Brightness
      scr=2; // Goto BackLight
    else if (scr==5) // Presently in Settings
    {
      if (mar==10)
      {
        if (ana)  // Analog / digital
          ana=false;
        else
          ana=true;
        scr=0;
      }//if block
      if (mar==20)
        scr=6; // Goto Set Time
      if (mar==30)
        scr=7;// Goto Profiles
      if (mar==40)
        scr=1; // Goto Main Menu
    }//else if
     else if (scr==6) // Presently in Set Time
    {
      if (mar==18)
        mar=36; // Goto next option
      else if (mar==36)
        mar=54; // Goto next option
      else if (mar==54)
        scr=5; // Goto Settings
    }//else if
    else if (scr==7) // Presently in Profiles
    {
      if (mar==10)
      {
        slp=true; // Sleep mode
        offbl();
        scr=0; // Goto Time Screen
      }//if block
      if (mar==20)
      {
        tou=true; // Touch mode
        offbl();
        lcd.enableSleep();
        scr=0;
      }//if block
      if (mar==30)
      {
        // Bluetooth mode
        if (digitalRead(BT)==HIGH)
          digitalWrite(BT,LOW); // Turn off the BT
        else
          digitalWrite(BT,HIGH); // Turn on the BT
      }//if block
      if (mar==40)
        scr=1; // Goto Main Menu
    }//else if block
    else if (scr==8)
      scr=0; // Goto Time Screen
  }//if block
  lastval=val;
  // ---------------------------------------
  
  // -------- Screen Transistions ---------
  // Specific screen events 
  switch (scr)
  {
    case 0:
    if (!slp)
    {
      if (ana)
        anaclk();
      else
        digclk();
    }//if block
    if (slp)
    {
      lcd.enableSleep();
      offbl();
      s++;
    }//if block
    break;
    
    case 1:
    // ----------- Main Menu ------------
    mainmenu();
    break;
    
    case 2:
    // ----------- Back Light ------------
    backlight();
    break;
    
    case 3:
    // ------------ Set Brightness -----------
    setbl();
    break;
    
    case 4:
    // ----------- Game Flappy --------------
    flappy();
    break;
    
    case 5:
    // -------------- Settings -----------------
    settings();
    break;
    
    case 6:
    // --------------- Set Time -----------------
    settime();
    break;
    
    case 7:
    // ---------------- Profile -----------------
    profile();
    break;
    
    case 8:
    // -------------- Connected -----------------
    btconnected();
    break;
  }//switch block
  
  
  // --------- Timer ------------
  if (ms>=1000)
  {
    s+=((int)(ms/1000));
    ms=((int)(ms%1000));
  }//if block
  if (s>=60)
  {
    m+=((int)(s/60));
    s=((int)(s%60));
    if (scr==0)
    {
      lcd.clrScr(); 
      lcd.update();
    }//if block
  }//if block
  if (m>=60)
  {
    //h+=1;//(floor(m/60));
    switch (h)
    {
      case 0:
      h=1;
      hr=1;
      break;
      case 1:
      h=2;
      hr=2;
      break;
      case 2:
      h=3;
      hr=3;
      break;
      case 3:
      h=4;
      hr=4;
      break;
      case 4:
      h=5;
      hr=5;
      break;
      case 5:
      h=6;
      hr=6;
      break;
      case 6:
      h=7;
      hr=7;
      break;
      case 7:
      h=8;
      hr=8;
      break;
      case 8:
      h=9;
      hr=9;
      break;
      case 9:
      h=10;
      hr=10;
      break;
      case 10:
      h=11;
      hr=11;
      break;
      case 11:
      h=12;
      hr=12;
      break;
      case 12:
      h=0;
      hr=0;
      break;
    }//switch block
    m=((int)(m%60));
  }//if block
  if (h>=12)
  {
    h=((int)(h%12));
  }//if block
  //s++;
  if (flag)
    delay(1000);
  // ------------------------
  
  
}//loop()

void mainmenu()
{
  // Main Menu
  lcd.print("- Main Menu -",CENTER,1);
  lcd.print("1) Back Light",LEFT,10);
  lcd.print("2) Settings",LEFT,20);
  lcd.print("3) Games",LEFT,30);
  lcd.print("4) Back",LEFT,40);
  lcd.update();
  lcd.clrRect(0,mar-2,83,mar+10-2);
  mar=map(analogRead(A0),0,1019,10,40);
  mar=floor(mar/10)*10;
  lcd.drawRect(0,mar-2,83,mar+10-2);
  lcd.update();
  
  // ----- Timer -------
  ms+=1250;
  delay(250);
  if (ms>=1000)
  {
    s+=((int)(ms/1000));
    ms=((int)(ms%1000));
  }//if block
  // -------------------
  
}//mainmenu()
void anaclk()
{
  // Analog Clock
  lcd.clrLine(42,24,42+floor(20*sin(PI/180*6*s)),24-floor(20*cos(PI/180*6*s)));
  lcd.clrLine(42,24,42+floor(15*sin(PI/180*6*m)),24-floor(15*cos(PI/180*6*m)));
  lcd.clrLine(42,24,42+floor(10*sin(PI/180*((30*h)+(m/2)))),24-floor(10*cos(PI/180*((30*h)+(m/2)))));
  lcd.update();
  s++;
  // -- Timer --
  lcd.drawLine(42,24,42+floor(20*sin(PI/180*6*s)),24-floor(20*cos(PI/180*6*s)));
  lcd.drawLine(42,24,42+floor(15*sin(PI/180*6*m)),24-floor(15*cos(PI/180*6*m)));
  lcd.drawLine(42,24,42+floor(10*sin(PI/180*((30*h)+(m/2)))),24-floor(10*cos(PI/180*((30*h)+(m/2)))));
  lcd.update();
  
}///anaclk()
void digclk()
{
  // Digital Clock
  lcd.clrScr();
  String str="0";
  if (hr<1)
    str=str+"0";
  else
    str=str+hr;
  //if (h<10)
    //str="0"+str;
  str=str+":";
  if (m<10)
    str=str+"0";
  str=str+""+m+":";
  if (s<10)
    str=str+"0";
  str=str+""+s;
  lcd.print(str,CENTER,20);
  lcd.update();
  s++;
}//digclk()
void backlight()
{
  // Back Light Settings
  lcd.print("- Back Light -",CENTER,1);
  lcd.print("1) ON/OFF",LEFT,10);
  lcd.print("2) Set Brightness",LEFT,20);
  lcd.print("3) Power Savor",LEFT,30);
  lcd.print("4) Back",LEFT,40);
  lcd.update();
  lcd.clrRect(0,mar-2,83,mar+10-2);
  mar=map(analogRead(A0),0,1019,10,40);
  mar=floor(mar/10)*10;
  lcd.drawRect(0,mar-2,83,mar+10-2);
  lcd.update();
  
  // ----- Timer -------
  ms+=1250;
  delay(250);
  if (ms>=1000)
  {
    s+=((int)(ms/1000));
    ms=((int)(ms%1000));
  }//if block
  // -------------------
  
}//backlight()
void ldrbl()
{
  // Light Dependent Resistor Back Light
  // ****** edit the range later *************
  analogWrite(led,map(analogRead(A0),0,1023,0,255));
}//ldrbl()
void onbl()
{
  // ON Back Light
  digitalWrite(led,HIGH);
}//onbl()
void offbl()
{
  // OFF Back Light
  digitalWrite(led,LOW);
}//offbl()
void setbl()
{
  // Set Back Light
  lcd.print(" - Brightness - ",CENTER,1);
  lcd.drawLine(0,24,84,24);
  lcd.clrRect(map(temp,0,1023,0,84)-2,20,map(temp,0,1023,0,84)+2,29);
  lcd.update();
  temp=analogRead(A0);
  lcd.drawRect(map(temp,0,1023,0,84)-2,20,map(temp,0,1023,0,84)+2,29);
  lcd.update();
  analogWrite(led,map(temp,0,1023,0,255));
  s++;
}//setbl()
boolean captouch()
{
  // Capacitive Touch Sensor
  long start = millis();
  long total1 =  cs_4_2.capacitiveSensor(30);
  // Edit Timing
  if (total1>200&&((millis()-start)<10))
    return true;
  else
    return false;
}//captouch
void flappy()
{
  lcd.clrScr();
  lcd.update();
  flag=false;
  for (int i=0;i<5;i++)
    lcd.drawRect(x-2+i,y-2+i,x+5-2-i,y+5-2-i);
  lcd.drawRoundRect(a-2,b-10,a+3,b+10);
  lcd.printNumI(score,LEFT,40);
  lcd.update();
  
  y=map(analogRead(A0),0,1023,0,48);
  a-=spd;
  if (a>0&&a<100)
    once=true;
  if (once)
  {
    if (a<0||a>200)
    {
      score+=5;
      a=90;
      b=random(48);
      once=false;
    }//if block
  }//if block
  if (score%25==0&&score>0)
  {
    score+=5;
    spd++;
    x+=5;
  }//if block
  if (abs(a-x)<=6&&abs(b-y)<=13)
  {
    if (sqrt(((a-x)*(a-x))+((b-y)*(b-y)))<(13.843))
    {
      for (int i=0;i<3;i++)
      {
        lcd.invert(true);
        delay(500);
        lcd.invert(false);
        delay(500);
        s++;
      }//for loop
      flag=true;
      lcd.clrScr();
      lcd.update();
      a=100;  // Coordinates of enemy
      b=24;  // Coordinates of enemy
      x=15; // Coordinates of player
      y=19; // Coordinates of player
      spd=1;
      score=0;
      scr=1;
    }//if block
  }//if block
  
  // ----- Timer -------
  ms+=50;
  delay(50);
  if (ms>=1000)
  {
    s+=((int)(ms/1000));
    ms=((int)(ms%1000));
  }//if block
  // -------------------
  
}//flappy()
void settings()
{
  // Settings
  lcd.print("- Settings -",CENTER,1);
  lcd.print("1) Analog/Digital",LEFT,10);
  lcd.print("2) Set Time",LEFT,20);
  lcd.print("3) Profile",LEFT,30);
  lcd.print("4) Back",LEFT,40);
  lcd.update();
  lcd.clrRect(0,mar-2,83,mar+10-2);
  mar=map(analogRead(A0),0,1019,10,40);
  mar=floor(mar/10)*10;
  lcd.drawRect(0,mar-2,83,mar+10-2);
  lcd.update();
  
  // ----- Timer -------
  ms+=1250;
  delay(250);
  if (ms>=1000)
  {
    s+=((int)(ms/1000));
    ms=((int)(ms%1000));
  }//if block
  // -------------------
  
}//settings()
void settime()
{
  
  lcd.print("- Set Time -",CENTER,1);
  
  // Digital Clock
  lcd.clrScr();
  String str="0";
  if (hr<1)
    str=str+"0";
  else
    str=str+hr;
  str=str+":";
  if (m<10)
    str=str+"0";
  str=str+""+m+":";
  if (s<10)
    str=str+"0";
  str=str+""+s;
  lcd.print(str,CENTER,20);
  lcd.update();
  
  lcd.clrRect(mar-1,20-2,mar+12+1,28+1);
  //mar=map(analogRead(A0),0,1010,18,54);
  mar=floor(mar/18)*18;
  if (mar<1)
    mar=18;
  lcd.drawRect(mar-1,20-2,mar+12+1,28+1);
  lcd.update();
  int tm=0;
  if (mar==18)
    tm=map(analogRead(A0),0,1023,0,12);
  else
    tm=map(analogRead(A0),0,1023,0,59);
  switch (mar)
  {
    case 18:
    hr=tm;
    h=hr;
    break;
    case 36:
    m=tm;
    break;
    case 54:
    s=tm;
    break;
  }//switch block
  
}//settime()
void profile()
{
  // Profile
  lcd.print("- Profile -",CENTER,1);
  lcd.print("1) Sleep",LEFT,10);
  lcd.print("2) Touch",LEFT,20);
  lcd.print("3) Bluetooth",LEFT,30);
  lcd.print("4) Back",LEFT,40);
  lcd.update();
  lcd.clrRect(0,mar-2,83,mar+10-2);
  mar=map(analogRead(A0),0,1019,10,40);
  mar=floor(mar/10)*10;
  lcd.drawRect(0,mar-2,83,mar+10-2);
  lcd.update();
  
  // ----- Timer -------
  ms+=1250;
  delay(250);
  if (ms>=1000)
  {
    s+=((int)(ms/1000));
    ms=((int)(ms%1000));
  }//if block
  // -------------------
  
}//profile()
void btconnected()
{
  lcd.print("- Connected -",CENTER,1);
  lcd.update();
  s++;
  switch(bt)
  {
    case '0':
    // Initialized
    ;
    break;
    case '1':
    // BackLight ON
    onbl();
    break;
    case '2':
    // BackLight OFF
    offbl();
    break;
    case '3':
    // Message Received
    lcd.print("Unread Message",LEFT,28);
    lcd.update();
    break;
    case '4':
    // Missed Call
    lcd.print("Missed Call",LEFT,38);
    lcd.update();
    break;
    case '5':
    // Back
    lcd.clrScr();
    lcd.update();
    scr=0;
    break;
  }//switch block
}//btconnected()
