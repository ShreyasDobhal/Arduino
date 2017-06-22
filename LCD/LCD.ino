#include <LiquidCrystal.h>
//pins : - 12 11 5 4 3 2 5V GND
//pins : - GND Vcc Vo RS RW E D4 D5 D6 D7
LiquidCrystal lcd (12,11,5,4,3,2);

void setup()
{
  lcd.begin(16,2);
  lcd.clear();
}

void loop()
{
  lcd.print("Printing Text");
  delay(3000);
  lcd.clear();
  
  lcd.setCursor(0,1);
  lcd.print("Setting Cursor");
  delay(3000);
  lcd.clear();
  
  lcd.print("Blinking Cursor");
  lcd.blink();
  delay(3000);
  lcd.clear();
  lcd.noBlink();
  
  lcd.print("uline cursor");
  lcd.cursor();
  delay(3000);
  lcd.clear();
  
  lcd.print("No Cursor");
  lcd.noCursor();
  delay(3000);
  lcd.clear();
  
  lcd.print("no display");
  delay(1000);
  lcd.clear();
  lcd.noDisplay();
  lcd.print("display on");
  delay(3000);
  lcd.display();
  delay(3000);
}
