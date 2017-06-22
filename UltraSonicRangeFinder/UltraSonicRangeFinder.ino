const int pingPin = 2; // Trig
const int pingin = 3;  // Echo
void setup() 
{
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  digitalWrite(12,LOW);
}
void loop()
{
  long duration, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingin, INPUT);
  duration = pulseIn(pingin, HIGH);

  cm = microsecondsToCentimeters(duration);
  
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  if (cm<=10)
  {
    digitalWrite(13,HIGH);
    //digitalWrite(11,LOW);
  }
  else
  {
    digitalWrite(13,LOW);
    //digitalWrite(11,HIGH);
  }
  delay(100);
}
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
