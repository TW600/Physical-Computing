# Experiment 5 - Human Machine Interface 

In this experiment, I want to understand how I can interact with the monitoring solution and get real time feedback on current data status and be alerted in case of any abnormal events. 

 Breaking this down into functions _ I want to understand the following functions.
 
•	A visual component to display real-time measured data.

•	An audible function to give feedback of normal operation and any abnormal operation

•	Ability to provide manual inputs via a button press

## LCD Display

A very common LCD display is the 16 Character by 2 line display, known as a 16 x 2.  These displays can have either a I2C serial  interface or an 8 bit  parallel data interface  

<img width="602" height="229" alt="(1)LCDdisplay" src="https://github.com/user-attachments/assets/05cb0614-26d9-49c7-bc39-c4ee1781b3f0" />

For simplicity we will use the I2C serial  interface. 

The display requires only 4 wires.

5V – Vin
 
0V – Gnd
 
SCL – Serial Clock
 
SDA  - Serial Data


To be able to communicate with the display, then an I2C display driver needs to be installed. This is simple to achieve. Install a standard display library using the Aduino library manager.

<img width="218" height="261" alt="(2)LibraryManager" src="https://github.com/user-attachments/assets/3dc98d34-bca8-4b6e-b877-4970f020a414" />

Once installed – then reference this library using an include, such as below.

    #include <LiquidCrystal_I2C.h>

As this in Serial display – then the I2C address of the display needs to be known and set up in the instantiation of the display module. In our example below, then the I2C address is 0x3F, and we define the display size as 16 X 2

•	LCD_I2C lcd(0x3F,16,2);

If the I2C address is not known, then it will be necessary to scan all address 0x00 – 0xFF to discover the address.

The installed library then provides a number of standard functions to configure the LCD. We will use the following basic functions

•	lcd.backlight(); - Turns LCD Backlight ON

•	lcd.noBacklight();  - Turns LCD Backlight OFF

•	lcd.setCursor (0,0); - Sets the active character to a local on the 16 x 2 display

•	lcd.print() ; Function  to write data to the display from the active character location

•	lcd.clear();  Clears all data from the display

This can then implemented in our code – for example we can print data variables as follows;

    lcd.clear();
     lcd.setCursor (0,0);
     if (display_cycle < 4){
     if (display_cycle == 0)
        {
           lcd.print("Temperature is ");
           lcd.setCursor (0,1);
           lcd.print(temp);         
        }
       }
##  Audible Buzzer

The Arduino supports various audible devices. The most popular type modules with Piezo buzzers that are driven directly by the Arduino to generate audible tones.

I chose a typical low cost buzzer for this experiment - [HALJIA Passive Low Level Trigger Buzzer Alarm Module Compatible with Arduino : Amazon.co.uk: Electronics & Photo](https://www.amazon.co.uk/dp/B071DFX4W2?ref=ppx_yo2ov_dt_b_fed_asin_title)

<img width="369" height="208" alt="(3)Buzzer" src="https://github.com/user-attachments/assets/fd2eb8f9-4a3a-4184-bd3c-8649caa63c38" />

This requires the I/O signal output to generate a frequency output with a 50% square wave duty cycle  to drive the buzzer, and allows a buzzer to be directly connected (See Schematic)

The typical specification is:-

<img width="642" height="207" alt="(4)BuzzerSpecification" src="https://github.com/user-attachments/assets/21fcd974-8f1e-468b-83cb-8db799582ead" />

Luckily, he Arduino has a built in standard library function called tone - [tone() | Arduino Documentation](https://docs.arduino.cc/language-reference/en/functions/advanced-io/tone/)

This function requires just 3 parameters

•	pin – Output pin on which to generate the tone.

•	Frequency - Frequency of the tone in hertz. 

•	Duration - Duration of the tone in milliseconds.

e.g. 

tone (13,1200,500); - Will output on GPIO13, a 1200hz square wave for 500 milli seconds
        
## Button Switch Input

One of the most basic and powerful functions of the Arduino Esp32 is the ability connect a button and use this as a Digital Input.  

This is described in the Arduino documentation  - [How to Wire and Program a Button | Arduino Documentation](https://docs.arduino.cc/built-in-examples/digital/Button/)

<img width="218" height="212" alt="(5)Button" src="https://github.com/user-attachments/assets/6094aa18-b54c-4cbc-bb1c-4d0fed8347b0" />

Every GPIO pin can be configured as either as an INPUT or OUTPUT. In this experiment we will connect a button to our pin, and define this as an INPUT as follows

    // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT);
    }
We can then read the status of this pin, by this simple Digital Read Command

    buttonState = digitalRead(buttonPin)

Now this is configured in our design, then our program can monitor the status of the button. In this experiment, if our button is pressed it will turn ON the LCD of our display. The intention is that the display is only needed to be read when someone is present, and can be OFF when not in use to save energy.

