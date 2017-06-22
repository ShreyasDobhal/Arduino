// Measure It 3D

int trig = 2; // Trig
int echo = 3;  // Echo
int btn = A0; // Button
int dist = 0; // Distance measured
long sum = 0; // Total distance
int i = 0; // Number of iterations
String val=""; // Data for Serial input

void setup() 
{
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  pinMode(btn,INPUT);
}//setup()

void loop()
{
  // Measuring distance based on serial event
  if (Serial.available()>0)
  {
    val="";
    while (Serial.available()>0)
      val=val+(char)Serial.read();
  }
  
  if (val=="stop")
    val="";
  else if (val=="1"||val=="on")
    digitalWrite(13,HIGH);
  else if (val=="0"||val=="off")
    digitalWrite(13,LOW);
  else if (val=="ok")
  {
    // measure the dist
    distance();
    // filtering and scaling the distance for sending
    if (dist<5)
      dist=5;
    else if (dist>=100)
      dist=99;
    Serial.println((char)(dist+28));
    val="";
  }//else if block
  else if (val=="dist")
  {
    // continuous distance measurement
    distance();
    Serial.println(dist);
  }//else if block
  
  
  // Measuring distance based on button event
  if (analogRead(btn)>500)
  {
    while (analogRead(btn)>500)
    {
      // Button pressed
      digitalWrite(13,HIGH);
      distance();
      if (dist>500)
        continue;
      sum += dist;
      i++;
    }//while loop
    // Button Released
    if (i!=0)
      dist = (int)(sum/i); // taking average
    i=0;
    sum=0;
    digitalWrite(13,LOW);
    Serial.println(dist);
  }//if block
  
  delay(100);
}//loop()

void distance()
{
  long duration;
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);
  pinMode(echo, INPUT);
  duration = pulseIn(echo, HIGH);
  dist = (int)(duration / 29 / 2);
}//distance()
