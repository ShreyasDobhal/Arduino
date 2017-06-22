#include <IRremote.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  irrecv.enableIRIn(); // Start the receiver
}//setup()

void loop() 
{
  if (irrecv.decode(&results)) 
  {
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
