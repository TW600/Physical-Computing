#include <Wire.h>

int lightsensor_adc_value = 0;
const int lightsensorpin = 36; //pin 36 is ADC1_channel0
const int adc_maximum_value = 4096; // 12 bit adc value from esp32
float light_level = 0;

void setup() {

  Serial.begin(115200); 
}

void loop() {

  lightsensor_adc_value = analogRead(lightsensorpin);// Read the ADC value 
  Serial.print("Light Value = ");
  Serial.println (lightsensor_adc_value);    
  
  light_level = ((float)lightsensor_adc_value/adc_maximum_value)*100; // express as a percentage of maximum ADC value
  Serial.print ( "Light level at ");
  Serial.print (light_level);
  Serial.println ("%");

  delay(1000);
}
