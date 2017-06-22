// Electromania

#include <LedControlMS.h>
#include <EEPROM.h>

// LED Matrix display
LedControl lc=LedControl(8,7,6,1);

// Global Variables
#define width 8 // length of LED matrix
#define height 8 // breadth of LED matrix
int lives, score; // lives and score of the player
boolean over; // variable to mark end of the game

// Permanent memory variables
byte highScore=0; // variable to store all time highscore
int address=0; // address where highscore is stored in memory

// Game Objects
#define nobj 4 // number of baskets in screen + 1
int x[nobj],y[nobj]; // x and y coordinates of baskets
double xpos[nobj];
double sp[nobj];
int len[nobj]; // their speeds and lengths

// Game Egg
int X,Y; // coordinates of the egg
int jumpHt = 3; // maximum jumping height
int jf=0,ji=0; // jumping flag and index

// Game Settings
long frameRate=10; // time lapse in millisec
long t1,t2; // initial and final times
int val=0; // value for reading input

// Game Area
boolean board[height][width]; // virtual board or screen

// Joystick Pin configuration
int right=A4,left=A5,up=A3;

// Light Sensor
int ldr=A2;

void setup()
{
  Serial.begin(9600); // Serial communication for debugging
  
  // LED Matrix Initialization
  lc.shutdown(0,false);
  lc.setIntensity(0,1);
  lc.clearDisplay(0);
  
  // High Score Initialization
  highScore = EEPROM.read(address);
  
  // Start Screen
  startScreen(); // displaying name of game
  delay(500);
  lc.clearDisplay(0);
}//setup()

void loop()
{
  delay(500);
  imageShow(1);
  delay(1000);
  gameInit(); // Initializes the game
  gamebegin(); // Game starts
  imageShow(2); // Game ended
  delay(1000);
  displayScore(); // displaying the score of the player
  keyPress(); // waiting for the player to respond
}//loop()

void gameInit()
{
  // Initializes game variables
  lives=3; // Default lives
  score=0;
  over=false;
  jf=ji=0;
  x[0] = width/2; // Placing the basket to the centre
  y[0] = 7; // Placing the basket at the bottom
  sp[0] = 0.0; // Giving it default speed of zero
  len[0] = 3; // Giving the default length of 3
  xpos[0] = (double)x[0];
  // Randomising other baskets
  for (int i=1;i<nobj;i++)
  {
    x[i] =randomNum(width-2)+1; // Random postion
    xpos[i] = (double)x[i];
    y[i] = y[i-1]-3;
    sp[i] = 0;// initially at rest
    len[i] = 3;
  }//for loop
  X = x[0]; // keeping the egg on the lowermost basket
  Y = y[0]-1;
  
  t1 = millis(); // starting time
  t2 = t1;
}//gameInit()

void gamebegin()
{
  while (1)
  {
    if (!over) // game is not over
    {
      if (t2-t1>=frameRate) // checking for frameRates without delay
      {
        t1 = t2;
        enterFrame(); // function responsible for spontaneous and continuos processes
      }//if block
      
      // Serial commmunication
      if (Serial.available())
        val = Serial.read();
      switch (val)
      {
        case 'a' :
          if (x[0]>0)
            x[0]--;
          if (jf==0)
            X=x[0];
          val=0;
          draw();
          break;
        case 'd' :
          if (x[0]<width-1)
            x[0]++;
          if (jf==0)
            X=x[0];
          val=0;
          draw();
          break;
        case 'w' :
          if (jf==0) // if egg is on the basket then only it can be thrown
          {
            jf=1; // flag value 1 denotes egg is moving upwards
            ji=0;
            tone(11,100,100); // Jumping sound
          }//if block
          val=0;
          draw();
          break;
      }//switch block
      
      // Reading the sensor values
      int leftVal = analogRead(left); // value read by the joystick in left position
      int rightVal = analogRead(right); // value read by the joystick in right position
      int upVal = analogRead(up); // value read by other joystick for throwing the egg
      
      // Joystick control
      if (leftVal>500) // Joystick moved left
      {
        if (x[0]>0) // if basket is inside the screen then only move
          x[0]--; // moving to the left
        if (jf==0) // if egg is not in air then only follow the basket
          X=x[0];
      }//if block
      else if (rightVal>500) // Joystick moved right
      {
        if (x[0]<width-1) // if basket is inside the screen then only move
          x[0]++; // moving to the right
        if (jf==0) // if egg is not in air then only follow the basket
          X=x[0];
      }//else if block
      if (upVal>500) // Joystick moved up
      {
        if (jf==0) // if egg is on the basket then only it can be thrown
        {
          jf=1; // flag value 1 denotes egg is moving upwards
          ji=0;
          tone(11,100,100); // Jumping sound
        }//if block
      }//else block
      
      t2 = millis(); // Current time
    }//if block
    else // Game Over
    {
      death(); // Death transition
      boardReset(); // screen is cleared
      draw();
      Serial.println(score); // Score serial printted
      return;
    }//if block
  }//while loop
}//gamebegin()

void enterFrame()
{
  for (int o=1;o<nobj;o++)
  {
    xpos[o]+=sp[o]; // moving every basket with respective speeds
    x[o]=(int)xpos[o];
    if ((x[o]+len[o]/2>width-1)||(x[o]-len[o]/2<0)) // bounding the motion of the baskets or setting screen as the limits
      sp[o]*=-1; // changing the direction of the moving baskets
  }//for loop
  if (jf==1) // egg is moving upwards
  {
    if (ji<jumpHt)
    {
      Y-=jf; // egg moving upwards
      ji++;
    }//if block
    else
    {
      jf=-1; // flag value of -1 represents egg is falling
      ji=0;
    }//else block
  }//if block
  if (jf==-1) // egg is falling freely
  {
    if (Y>height) // Egg missed
      missed();
    if (Y==y[1]-1&&X>=x[1]-len[1]/2&&X<=x[1]+len[1]/2)// Egg caught in basket
      caught();
    else // Egg still falling
      Y-=jf;
  }//if block
  autoBrightness(); // setting screen brightness depending upon the exposure
  draw(); // displaying the screen or game status
}//enterFrame()

void draw()
{
  boardReset();
  // Detecting the positions of the objects
  for (int i=0;i<height;i++)
    for (int j=0;j<width;j++)
      for (int o=0;o<nobj;o++)
        if (((i==y[o])&&(j>=x[o]-len[o]/2)&&(j<=x[o]+len[o]/2))||(j==X&&i==Y))
          board[i][j]=true;
  for (int i=0;i<height;i++)
  {
    for (int j=0;j<width;j++)
      lc.setLed(0,i,j,board[i][j]);
    delay(1);
  }//for loop
}//draw()

void boardReset()
{
  // clearing the board
  for (int i=0;i<height;i++)
    for (int j=0;j<width;j++)
      board[i][j]=false;
}//boardReset()

void missed()
{
  // egg was not caught
  // so penalty imposed
  lives--; // one life is deducted
  if (lives<=0) // if no more lives left
    over = true; // Game Over
  else // giving another chance to the player
  {
    // egg replaced on the basket
    tone(11,7500,100); // Fault tone played
    delay(1000);
    jf=0;
    ji=0;
    X=x[0];
    Y=y[0]-1;
  }//if block()
}//missed()

void caught()
{
  // egg was caught in the upper basket
  // centralising the egg on the basket
  X = x[1];
  tone (11,1500,50);
  // Scrolling objects down
  int diff = y[0]-y[1];
  for (int i=0;i<diff;i++)
  {
    for (int o=0;o<nobj;o++)
      y[o]++;
    Y++;
    // giving an animated effect of scrolling
    for (int ani=0;ani<10;ani++)
      draw();
  }//for loop
  // Swapping objects
  for (int i=0;i<nobj-1;i++)
  {
    x[i]=x[i+1];
    xpos[i]=xpos[i+1];
    y[i]=y[i+1];
    len[i]=len[i+1];
    sp[i]=sp[i+1];
  }//for loop
  sp[0]=0.0;
  // Setting random data
  x[nobj-1]=randomNum(width-2)+1;
  y[nobj-1]=y[nobj-2]-3;
  len[nobj-1]=3;
  //sp[nobj-1]=0.5;//random(3)-1;
  if (score<=10)
    sp[nobj-1] = (randomNum(7)-3)/25.0;
  else if (score<=25)
    sp[nobj-1] = (randomNum(score)-score/2)/25.0;
  else
    sp[nobj-1] = (randomNum(score)-score/2)/10.0;
  score++;
  jf=0;
}//caught()

void startScreen()
{
  // Egg Game Scrolling Text
  byte strt[height][47]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
  for (int k=0;k<47-width+1;k++)
  {
    for (int i=0;i<height;i++)
    {
      for (int j=0;j<width;j++)
        lc.setLed(0,i,j,strt[i][j+k]);
      delay(2);
    }//for loop
    delay(250);
  }//for loop
}//startScreen()

void death()
{
  for (int k=0;k<8;k++)
  {
    tone(11,7500,150);
    for (int i=0;i<height;i++)
      for (int j=0;j<width;j++)
        lc.setLed(0,i,j,board[i][j]);
    delay(100);
    for (int i=0;i<height;i++)
      for (int j=0;j<width;j++)
        lc.setLed(0,i,j,!board[i][j]);
    delay(100);
  }//for loop
}//death()

void imageShow(int x)
{
  byte img[height][width];
  switch (x)
  {
    case 1:
      // Happy Face
      {
      byte tmpimg[height][width]={{0,0,0,0,0,0,0,0},
                                  {0,1,1,0,0,1,1,0},
                                  {0,1,1,0,0,1,1,0},
                                  {0,0,0,0,0,0,0,0},
                                  {0,0,0,0,0,0,0,0},
                                  {0,1,0,0,0,0,1,0},
                                  {0,0,1,1,1,1,0,0},
                                  {0,0,0,0,0,0,0,0}};
      for (int i=0;i<height;i++)
        for (int j=0;j<width;j++)
          img[i][j]=tmpimg[i][j];
      }
      break;
    case 2: 
      // Sad Face
      {
      byte tmpimg[height][width]={{0,0,0,0,0,0,0,0},
                                  {0,1,1,0,0,1,1,0},
                                  {0,1,1,0,0,1,1,0},
                                  {0,0,0,0,0,0,0,0},
                                  {0,0,0,0,0,0,0,0},
                                  {0,0,1,1,1,1,0,0},
                                  {0,1,0,0,0,0,1,0},
                                  {0,0,0,0,0,0,0,0}};
      for (int i=0;i<height;i++)
        for (int j=0;j<width;j++)
          img[i][j]=tmpimg[i][j];
      }
      break;
    case 3: 
      // Up Arrow
      {
      byte tmpimg[height][width]={{0,0,0,0,0,0,0,0},
                                  {0,0,0,1,1,0,0,0},
                                  {0,0,1,0,0,1,0,0},
                                  {0,1,0,0,0,0,1,0},
                                  {0,0,0,1,1,0,0,0},
                                  {0,0,1,0,0,1,0,0},
                                  {0,1,0,0,0,0,1,0},
                                  {0,0,0,0,0,0,0,0}};
      for (int i=0;i<height;i++)
        for (int j=0;j<width;j++)
          img[i][j]=tmpimg[i][j];
      }
      break;
    case 4: 
      // Up Arrow Animated
      {
      byte tmpimg[height][width]={{0,0,0,1,1,0,0,0},
                                  {0,0,1,0,0,1,0,0},
                                  {0,1,0,0,0,0,1,0},
                                  {0,0,0,1,1,0,0,0},
                                  {0,0,1,0,0,1,0,0},
                                  {0,1,0,0,0,0,1,0},
                                  {0,0,0,0,0,0,0,0},
                                  {0,0,0,0,0,0,0,0}};
      for (int i=0;i<height;i++)
        for (int j=0;j<width;j++)
          img[i][j]=tmpimg[i][j];
      }
      break;
  }//switch block
  for (int i=0;i<height;i++)
    for (int j=0;j<width;j++)
      lc.setLed(0,i,j,img[i][j]);
}//imageShow()

void won()
{
  for (int a=0;a<4;a++)
  {
    lc.clearDisplay(0);
    for (int i=a;i<height-a;i++)
    {
      lc.setLed(0,a,i,true);
      lc.setLed(0,height-a-1,i,true);
      lc.setLed(0,i,a,true);
      lc.setLed(0,i,height-a-1,true);
    }//for loop
    delay(250);
  }//for loop
  for (int a=3;a>=0;a--)
  {
    lc.clearDisplay(0);
    for (int i=a;i<height-a;i++)
    {
      lc.setLed(0,a,i,true);
      lc.setLed(0,height-a-1,i,true);
      lc.setLed(0,i,a,true);
      lc.setLed(0,i,height-a-1,true);
    }//for loop
    delay(250);
  }//for loop
}//won()

int randomNum(int num)
{
  int t=0;
  int l=analogRead(0); // this PORT must be left unconnected
  for (int i=0;i<l;i++)
    t=random(num);
  t=random(num);
  return t;
}//randomNum

void displayScore()
{
  byte num[10][height][4]={{{0,0,0,0}, // Zero
                            {1,1,1,1},
                            {1,0,0,1},
                            {1,0,0,1},
                            {1,0,0,1},
                            {1,1,1,1},
                            {0,0,0,0},
                            {0,0,0,0}},
                           {{0,0,0,0}, // One
                            {0,0,1,0},
                            {0,0,1,0},
                            {0,0,1,0},
                            {0,0,1,0},
                            {0,0,1,0},
                            {0,0,0,0},
                            {0,0,0,0}},
                           {{0,0,0,0}, // Two
                            {1,1,1,1},
                            {0,0,0,1},
                            {1,1,1,1},
                            {1,0,0,0},
                            {1,1,1,1},
                            {0,0,0,0},
                            {0,0,0,0}},
                           {{0,0,0,0}, // Three
                            {0,1,1,1},
                            {0,0,0,1},
                            {0,0,1,1},
                            {0,0,0,1},
                            {0,1,1,1},
                            {0,0,0,0},
                            {0,0,0,0}},
                           {{0,0,0,0}, // Four
                            {1,0,0,1},
                            {1,0,0,1},
                            {1,1,1,1},
                            {0,0,0,1},
                            {0,0,0,1},
                            {0,0,0,0},
                            {0,0,0,0}},
                           {{0,0,0,0}, // Five
                            {1,1,1,1},
                            {1,0,0,0},
                            {1,1,1,1},
                            {0,0,0,1},
                            {1,1,1,1},
                            {0,0,0,0},
                            {0,0,0,0}},
                           {{0,0,0,0}, // Six
                            {1,1,1,1},
                            {1,0,0,0},
                            {1,1,1,1},
                            {1,0,0,1},
                            {1,1,1,1},
                            {0,0,0,0},
                            {0,0,0,0}},
                           {{0,0,0,0}, // Seven
                            {1,1,1,1},
                            {0,0,0,1},
                            {0,0,0,1},
                            {0,0,0,1},
                            {0,0,0,1},
                            {0,0,0,0},
                            {0,0,0,0}},
                           {{0,0,0,0}, // Eight
                            {1,1,1,1},
                            {1,0,0,1},
                            {1,1,1,1},
                            {1,0,0,1},
                            {1,1,1,1},
                            {0,0,0,0},
                            {0,0,0,0}},
                           {{0,0,0,0}, // Nine
                            {1,1,1,1},
                            {1,0,0,1},
                            {1,1,1,1},
                            {0,0,0,1},
                            {1,1,1,1},
                            {0,0,0,0},
                            {0,0,0,0}}};
                            
  boolean feed=false;
  // Checking if highscore record is broken
  if (score>highScore)
  {
    // New HighScore
    feed=true;
    won();
    highScore=score;
    EEPROM.write(address,highScore);
  }//if block  
  
  if (feed||score>=100)
  {
    // High score created or
    // Score has more than two digits therefore scrolling required
    int cnt=0;
    // counting digits
    for (int tmp=score;tmp!=0;tmp/=10,cnt++);
    int arrLen=8+cnt*4+8;
    byte scoreArr[height][arrLen];
    for (int i=0;i<height;i++)
      for (int j=0;j<arrLen;j++)
        scoreArr[i][j]=0;
    int n=score;
    for (int k=1;k<=cnt;k++)
    {
      int tmp=n%10;
      for (int i=0;i<height;i++)
        for (int j=0;j<4;j++)
          scoreArr[i][arrLen-8-k*4+j]=num[tmp][i][j];
      n/=10;
    }//for loop
    
    for (int k=0;k<arrLen-width+1;k++)
    {
      for (int i=0;i<height;i++)
      {
        for (int j=0;j<width;j++)
          lc.setLed(0,i,j,scoreArr[i][j+k]);
        delay(2);
      }//for loop
      delay(200);
    }//for loop
    imageShow(3);
  }//else block
  if (score<100)
  {
    // Score is a two digit number so no need of scrolling
    int t=0,u=0;
    t=score/10; // Tens
    u=score%10; // Units
    for (int i=0;i<height;i++)
      for (int j=0;j<width;j++)
      {
        if (j<4)
          lc.setLed(0,i,j,num[t][i][j]);
        else
          lc.setLed(0,i,j,num[u][i][j-4]);
      }//for loop
  }//if block
  delay(2000);
}//displayScore()

void keyPress()
{
  // waiting for player to respond to continue game
  long time1=millis(); // initial time
  long time2=time1;
  int flagAni=0,aniFrame=3;
  while (1)
  {
    if (flagAni==0&&time2-time1>5000)
    {
      // waitting for more than  5sec
      // telling user to continue
      imageShow(aniFrame);
      flagAni=1;
      time1=time2;
    }//if block
    if (flagAni==1&&time2-time1>500)
    {
      // start animation
      if (aniFrame==3)
        aniFrame=4;
      else
        aniFrame=3;
      imageShow(aniFrame);
      time1=time2;
    }//if block
    if (Serial.available()>0)
      return;
    if (analogRead(up)>500)
      return;
    delay(2);
    time2=millis();
  }//while loop
}//keyPress()

void autoBrightness()
{
  int ldrVal = analogRead(ldr);
  lc.setIntensity(0,ldrVal*8/1024);
}//autoBrightness()
