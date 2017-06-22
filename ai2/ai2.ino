
String Input_Appinventor_String = "";         // String to hold incoming App inventor Data 


const int ledPin = 13;
int Valeur = 0 ;

void setup() {
  Serial.begin(9600); //in full version you can change it --> change it in App Inventor 2
    // Pin 13 has an LED connected on most Arduino boards:
  pinMode(ledPin, OUTPUT);
  Input_Appinventor_String.reserve(200);
  delay(2000);
}

void loop() {
   
  delay(10);

}

void serialEvent() {
  while (Serial.available()) { 
   
    // Read incomming Control From App Inventor
    char inChar = (char)Serial.read(); 
    // add it to the Input_Appinventor_String:
    Input_Appinventor_String += inChar;

 
     if (Contains(Input_Appinventor_String ,"Read")){         // If App Inveor Control is "Read"
      Serial.println ("Message from Arduino");             // Send a message to App Invetor get result
      
      Input_Appinventor_String ="";
   }
      
  
      if (Contains(Input_Appinventor_String ,"Right")){            // If App Inveot Control is "ON"
      digitalWrite(ledPin, HIGH);                      // Led 13 On
      Input_Appinventor_String ="";}
      
      if (Contains(Input_Appinventor_String ,"Left")){             // If App Inveot Control is "OFF"
      digitalWrite(ledPin, LOW);                        // Led 13 Off
      Input_Appinventor_String ="";}
   
  }
    
}

bool Contains( String s, String search) {
    int max = s.length() - search.length();  
    for (int i=0; i<= max; i++) 
    {
    if (s.substring(i) == search) return true;  // or i
    }
    return false;  //or -1
    }
