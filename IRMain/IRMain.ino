#include <IRremote.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  pinMode(3,OUTPUT);
  digitalWrite(3,LOW);
  pinMode(4,OUTPUT);
  digitalWrite(4,LOW);
  irrecv.enableIRIn(); // Start the receiver
}//setup()

void loop() 
{
  if (irrecv.decode(&results)) 
  {
    if (results.value==0x7295A904)
    {
      Serial.println("OK");
      digitalWrite(2,HIGH);
      digitalWrite(4,LOW);
    }
    if (results.value==0xEC27D43D)
    {
      Serial.println("UP");
      digitalWrite(2,LOW);
      digitalWrite(4,HIGH);
    }
    if (results.value==0x86BD99C)
    {
      Serial.println("Down");
      digitalWrite(2,HIGH);
      digitalWrite(4,HIGH);
    }
    if (results.value==0x1A422E43)
    {
      Serial.println("Left");
    }
    if (results.value==0xA23BD824)
    {
      Serial.println("Right");
    }
    if (results.value==0xF124B0B6)
    {
      Serial.println("On/Off");
      digitalWrite(2,LOW);
      digitalWrite(3,LOW);
      digitalWrite(4,LOW);
    }
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
