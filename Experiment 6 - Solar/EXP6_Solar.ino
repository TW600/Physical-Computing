#include <Wire.h>
#include <LCD_I2C.h>
#define ADCRANGE 4096 //12bit ADC = 4096
#define MAXOUTPUT 5 // 5V max output for battery voltage
const int batteryPin = 34; 

 LCD_I2C lcd(0x3F,16,2);
 int batteryVoltageADC;
 float batteryVoltage;

 void setup() {

  Serial.begin(115200); 
  lcd.begin();
  lcd.backlight();  //turns on backlight 
}

void loop() {

lcd.clear();
lcd.setCursor (0,0); // reset cursor to character 0 line 0 
batteryVoltageADC = analogRead(batteryPin); // Read ADC value
batteryVoltage = (float)batteryVoltageADC/ADCRANGE*MAXOUTPUT; // Convert this to a voltage

lcd.print("Battery Voltage =");
lcd.setCursor (0,1);
lcd.print(batteryVoltage); // display voltage to 2dp

lcd.setCursor (5,1);
lcd.print("Volts");

delay(1000);
}