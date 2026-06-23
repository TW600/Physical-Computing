#include <Wire.h>
#include <LCD_I2C.h>

#define buzzer 15 // GPIO pin 15
#define switch_input1 2 //GPIO pin 2

// //tone frequency definitions
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

bool switch1_state = false;

 LCD_I2C lcd(0x3F,16,2);
 int display_cycle = 0;

 void setup() {

  Serial.begin(115200); 
  pinMode(switch_input1, INPUT); //setup Gpio02 as input for switch
  pinMode(buzzer,OUTPUT); //setup gpio15 as output for buzzer
   
  lcd.begin();
  lcd.backlight();  //turns on backlight 
  lcd.setCursor (0,0);
  lcd.print("This is a test");  // print to screen 
  lcd.setCursor (0,1);
  lcd.print("of performance"); 
  //tone(buzzer, 1000); // basic buzzer //tone with no defined duration
  delay(3000);  // delay for 3 seconds 
  //notone(buzzer); //turn off buzzer
}

void loop() {

lcd.clear();
lcd.setCursor (0,0); // reset cursor to character 0 line 0 

// Test of input button switch functionality.
switch1_state = digitalRead(switch_input1);// read input
if (switch1_state == true)
{
  lcd.backlight();  //turns on backlight 
  Serial.println ("Button Pressed");
} 
else
{
  lcd.noBacklight(); // turn off backlight
  Serial.println ("Button Released");
} 
 


if (display_cycle < 4)
{
  if (display_cycle == 0)
  {
    lcd.print("My Name is ");
    lcd.setCursor (0,1);
    lcd.print("Tom White");      
    //tone(buzzer, 1000,200);   
    //tone(buzzer, 1200,200);   
    //tone(buzzer, 1400,200);   
    //tone(buzzer, 1600,200);   
 
  }

  if (display_cycle == 1)
  {
    lcd.print("I live in  ");
    lcd.setCursor (0,1);
    lcd.print("Bath");
    //tone(buzzer, 1600,200);   
    //tone(buzzer, 1400,200);   
    //tone(buzzer, 1200,200);   
    //tone(buzzer, 1000,200);   
  }
  if (display_cycle == 2)
  {
    lcd.print("I enjoy ");
    lcd.setCursor (0,1);
    lcd.print("Cybersecurity");
//star wars :-)
    //tone(buzzer,a, 500);
    //tone(buzzer,a, 500); 
    //tone(buzzer,a, 500);
    //tone(buzzer,f, 350);
    //tone(buzzer,cH, 150); 
    //tone(buzzer,a, 500);
    //tone(buzzer,f, 350);
    //tone(buzzer,cH, 150);
    //tone(buzzer,a, 650);
    delay (500);
}
  if (display_cycle == 3)
  {
    lcd.print("I want to get ");
    lcd.setCursor (0,1);
    lcd.print("Top Marks !!");
//star wars :-))
    //tone(buzzer,eH, 500);
    //tone(buzzer,eH, 500);
    //tone(buzzer,eH, 500); 
    //tone(buzzer,fH, 350);
    //tone(buzzer,cH, 150);
    //tone(buzzer,gS, 500);
    //tone(buzzer,f, 350);
    //tone(buzzer,cH, 150);
    //tone(buzzer,a, 650);
    delay(500);

  }
   
    display_cycle++;
   
   delay (2000);
}
else
{
  display_cycle = 0;
}

}
  