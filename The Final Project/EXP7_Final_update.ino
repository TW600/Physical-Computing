#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <LCD_I2C.h>

Adafruit_BME280 bme; //i2C driver interface for BME280
LCD_I2C lcd (0x3F,16,2); // I2C display used address 0x3F

// Stepper Motor definitions
#define STEPPER_PIN_1 12
#define STEPPER_PIN_2 14
#define STEPPER_PIN_3 27
#define STEPPER_PIN_4 26

#define buzzer 15 // GPIO pin 15
#define switch_input1 2 //GPIO pin 2

#define ADCRANGE 4096 //12bit ADC = 4096
#define MAXOUTPUT 5 // 5V max output for battery voltage

// tone frequency definitions (star Wars)
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

int stepnumber = 0;
int lightsensor_adc_value = 0;
int display_cycle = 0;

int Alarm_temp;

int batteryVoltageADC;
float batteryVoltage;
float Light_level;

const int batterymonitorpin = 34; //pin 34 is ADC1_channel6
const int lightsensorpin = 35; //pin 35 is ADC1_channel7

 
// Network credentials Here
const char* ssid     = "YourSSID"; //joins existing network
const char* password = "YourPassword"; //joins existing network
const char* soft_ap_ssid     = "ESP32-Network"; //for AP mode
const char* soft_ap_password = "Esp32-Password"; // for AP mode

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//variables to store the current LED states

String statePin16 = "off";
String statePin17 = "off";

//Output variable to GPIO pins
const int ledPin16 = 16;
const int ledPin17 = 17;

float temp, pressure, humidity;

// Current time
unsigned long currentTime = millis();

// Previous time
unsigned long previousTime = 0;

// Define timeout time in milliseconds
const long timeoutTime = 2000;

bool step_direction = false;
bool start = false;

bool switch1_state = false;
 
void setup() {

  Alarm_temp = 26; // Hardcoded Temperature alarm
  pinMode(buzzer,OUTPUT); //setup gpio15 as output for buzzer
  tone(buzzer, 1200,500); // basic buzzer tone for 0.5 second
  tone(buzzer, 1500,500); // basic buzzer tone for 0.5 second
  
  Serial.begin(115200); 
  lcd.begin();
  lcd.backlight();
  lcd.setCursor (0,0);
  lcd.print("A Final Project ");
  lcd.setCursor (0,1);
  lcd.print("Test Check  ");
  lcd.print(Alarm_temp); // Display Alarm Threshold value
  
 
  bool communication = bme.begin(0x76); // 0x76 is the address for the BME_280 sensor

   if (!communication) {
     Serial.println("Could not find a valid BME280 sensor");
     Serial.println("check wiring, address, sensor ID!");
     Serial.print("SensorID was: 0x");
     Serial.println(bme.sensorID(), 16);
     Serial.println("ID of 0xFF probably means a bad address\n");
     delay(10);
   }
   else {
     Serial.println("Communication established!\n");
   }


  pinMode(STEPPER_PIN_1, OUTPUT);      // set the Stepper pin mode
  pinMode(STEPPER_PIN_2, OUTPUT);      // set the Stepper pin mode
  pinMode(STEPPER_PIN_3, OUTPUT);      // set the Stepper pin mode
  pinMode(STEPPER_PIN_4, OUTPUT);      // set the Stepper pin mode

  pinMode(ledPin16, OUTPUT);      // set the LED pin mode
  digitalWrite(ledPin16, 0);      // turn LED off by default
  pinMode(ledPin17, OUTPUT);      // set the LED pin mode
  digitalWrite(ledPin17, 0);      // turn LED off by default

  WiFi.mode (WIFI_AP_STA); // Set up Simulataneus mode for STA + AP access
  WiFi.begin(ssid,password);// Tell Wifi to join with provided credentials for STA mode
  WiFi.softAP(soft_ap_ssid,soft_ap_password); // additionally tell Wifi to generate an Access point with these credentials

  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }

  // Print IP address and start web server
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println(WiFi.localIP());

  server.begin(); //starts Web Server
}

 

void loop() {

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client


    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off - used for DEBUG
            if (header.indexOf("GET /16/on") >= 0) {
              statePin16 = "on";
              digitalWrite(ledPin16, HIGH);               // Inidcates the stepper motor direction clockwise if LED on
            } else if (header.indexOf("GET /16/off") >= 0) {
              statePin16 = "off";
              digitalWrite(ledPin16, LOW);                // Inidicate the stepper motor direction anticlockwise off
            }
  
            // turns the GPIOs on and off - used for DEBUG
            
            if (header.indexOf("GET /17/on") >= 0) {
              statePin17 = "on";
              digitalWrite(ledPin17, HIGH);               // Drives stepper motor, indicates the LED on
            } else if (header.indexOf("GET /17/off") >= 0) {
              statePin17 = "off";
              digitalWrite(ledPin17, LOW);                //turns the LED off
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // CSS to style the on/off buttons
            client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: yellowgreen; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 32px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: gray;}</style></head>");

            client.println("<body><h1>Tom White Web Server</h1>");
            client.println("<p>Remote Monitoring Control Status</p>");
            client.println("<p></p>");

            client.println("<p>Temperature is </p>");
            client.println(temp);
            client.println(" *C");

            client.println("<p>Pressure is </p>");
            client.println(pressure);
            client.println(" hPa");

            client.println("<p>Humdity is </p>");
            client.println(humidity);
            client.println(" %");

            client.println("<p>Light Level is </p>");
            client.println(Light_level);
            client.println(" %");
            
            client.println("<p>Battery at </p>");
            client.println(batteryVoltage);
            client.println(" Volts");

 

            if (statePin16 == "off") {//Sets Stepper motor in Clockwise direction 
              client.println("<p><a href=\"/16/on\"><button class=\"button\">Forward</button></a></p>");
              step_direction = true;

            } else {//Sets Stepper motor in anti-Clockwise direction 
              client.println("<p><a href=\"/16/off\"><button class=\"button button2\">Reverse</button></a></p>");
              step_direction = false;
            }

            if (statePin17 == "off") {//Starts moving stepper mote
              client.println("<p><a href=\"/17/on\"><button class=\"button\">Start</button></a></p>");
              start = true;
            } else {//Stops moving stepper motor
              client.println("<p><a href=\"/17/off\"><button class=\"button button2\">Stop</button></a></p>");
              start = false;
            }

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;

          } else { // if you got a newline, then clear currentLine

            currentLine = "";

          }

        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }

    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  if (start == true){
    OneStep(step_direction);
    delay(20);
  }

  else

 {
    switch1_state = digitalRead(switch_input1);// read button input
    if (switch1_state == true)
    {
      lcd.backlight(); //Turn on LCD backlight - as someone present
      Serial.println ("Button Pressed");
    }
    else
    {
      lcd.noBacklight(); // turn off backlight
      Serial.println ("Button Released");
    }


     Serial.print("Temperature = ");
     temp = bme.readTemperature();
     Serial.print(temp);
     Serial.println(" *C");

     pressure = (bme.readPressure() /100.0F);
     Serial.print("Pressure = ");
     Serial.print(pressure);
     Serial.println(" hPa");
     Serial.print("Humidity = ");
     humidity = bme.readHumidity();
     Serial.print(humidity);
     Serial.println(" %\n");
     delay(1000);

 

     lightsensor_adc_value = analogRead(lightsensorpin); // Read ADC Value for Light sensor
     Serial.print("Light Value = ");
     Serial.println (lightsensor_adc_value);
     Light_level = (float)lightsensor_adc_value/ADCRANGE*100; // express as % of max light level
     Serial.println (Light_level);
     

     batteryVoltageADC = analogRead(batterymonitorpin); // Read ADC value for Battery Voltage
     Serial.print("Battery ADC Value = ");
     Serial.println (batteryVoltageADC);

     

     if (display_cycle < 5)
     {
        lcd.clear();
        lcd.setCursor (0,0);

        if (display_cycle == 0)
        {
           lcd.print("Temperature is ");
           lcd.setCursor (0,1);
           lcd.print(temp);        
           lcd.setCursor(5,1);
           lcd.print(" Deg C");
          if (temp > Alarm_temp){ // This is the alarm temperature

            tone(buzzer,a, 500);// Plays Star was tune :-)
            tone(buzzer,a, 500); 
            tone(buzzer,a, 500);
            tone(buzzer,f, 350);
            tone(buzzer,cH, 150); 
            tone(buzzer,a, 500);
            tone(buzzer,f, 350);
            tone(buzzer,cH, 150);
            tone(buzzer,a, 650);
            delay (500);
            tone(buzzer,eH, 500);
            tone(buzzer,eH, 500);
            tone(buzzer,eH, 500); 
            tone(buzzer,fH, 350);
            tone(buzzer,cH, 150);
            tone(buzzer,gS, 500);
            tone(buzzer,f, 350);
            tone(buzzer,cH, 150);
            tone(buzzer,a, 650);
            delay(500);
          }
        }

         if (display_cycle == 1)
         {
           lcd.print("Pressure is ");
           lcd.setCursor (0,1);
           lcd.print(pressure);
           lcd.setCursor(7,1);
          lcd.print(" Pa");
          }

         if (display_cycle == 2)
         {
           lcd.print("Humidity is ");
           lcd.setCursor (0,1);
           lcd.print(humidity);
           lcd.setCursor(5,1);
           lcd.print(" %RH");
         }

         if (display_cycle == 3)
         {
           lcd.print("Light Level is ");
           lcd.setCursor (0,1);
           lcd.print(Light_level);
           lcd.setCursor (5,1);     //Display light level to 2 dp 
           lcd.print("%");
         }
        
        if (display_cycle == 4)
        {
          batteryVoltage = (float)batteryVoltageADC/ADCRANGE*MAXOUTPUT; // Convert this to a voltage
          lcd.print("Battery Voltage =");
          lcd.setCursor (0,1);
          lcd.print(batteryVoltage); // display voltage to 2dp
          lcd.setCursor (5,1);
          lcd.print("Volts");
        }
        display_cycle++;
      }
      else
      {
        display_cycle = 0;
        tone(buzzer, 1000,200);   
        tone(buzzer, 1200,200);   
        tone(buzzer, 1400,200);   
        tone(buzzer, 1600,200);   
      }


  }

}

 
/////////////Stepper Motor Driver Function ///////////////////////////
void OneStep (bool dir){

    if (dir){ //FORWARD

      switch(stepnumber){

        case 0:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;

        case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;

        case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;

        case 3:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
        }

    }

    else{//REVERSE

     switch (stepnumber){

      case 0:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;

        case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;

        case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;

        case 3:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      }    
     }
   

  stepnumber++;
  if (stepnumber>3)
  {
    stepnumber =0;
  }

}



