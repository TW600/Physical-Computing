# Final Project 

## Description 
This is a remote monitoring station, that measures sunlight, temperature, humidity & pressure and is designed to operate remotely and provide data via a WiFi WAN link and be powered independently using solar energy.  The design can be configured to generate an alarm should a monitored value exceed a preset limit.  This allows it to be deployed in outdoor areas to monitor the environment.  The design additionally has a local display interface to provide data and any alarm conditions.

## Main Components
The design comprises of the following major components

•	Arduino ESP32WROOM

•	BME280 Sensor (Temperature, Humidity & Atmospheric Pressure)

•	Light Sensor

•	Buzzer 

•	LCD Display

•	Button

•	Stepper Motor & Stepper Motor Driver

•	Solar Panel & Battery

•	Solar Charger with 5V output

The above components were connected in line with [the schematic design](https://github.com/BSU-tangible-computing/physical-computing-24-25-TW6000/blob/4a6d716aa144650ecd6058a693dbabd14c348c53/The%20Final%20Project/EXP7_Final_Design_Schematic.png) 

The **ESP32 WROOM** is a high performance module with built in WiFi capability that is supported module within the Arduino IDE. It has dual core processing, 32 GPIO pins, support for I2C serial protocol, 12-bit ADC channels and low power modes. This makes it very suitable for industrial automation applications, such as this project.

The board used for this project is the ESP32 Dev kit4 module manufactured by Espressif. This module allows it to be directly plugged into a breadboard and be connected with jumper wires.

 See  [ESP32-DevKitC V4 Getting Started Guide - ESP32 - — ESP-IDF Programming Guide release-v4.2 documentation](https://docs.espressif.com/projects/esp-idf/en/release-v4.2/esp32/hw-reference/esp32/get-started-devkitc.html)

<img width="453" height="267" alt="(1)ESP32" src="https://github.com/user-attachments/assets/aa11a87b-7d78-478a-beb8-65518f4801bc" />

This module also provides both 3.3V and 5V outputs, although the I/O pins are only 3.3V tolerant. 

The datasheet describing all supported functionality is [esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf)

### Implementation
This module will be fitted to a breadboard, and supplied with external 5V power from the power supply rail on to the 5V pin.  The module will then generate a 3.3V output, which will be connected to 3.3V power supply rail. All GND connections will be made to the ground rails.

Each component will then be connected to the ESP32 to use the relevant functionality, based on the schematic diagram in the Github repository and following the ESP32 pinout as below.

<img width="602" height="252" alt="(2)ESP32pinSchematic" src="https://github.com/user-attachments/assets/c8662453-3714-4cc8-9e91-f1e73733b117" />

**The BME280 module** provides temperature, humidity & pressure via an I2C interface. The sensor itself is manufactured by Bosch and is a MEMS (Micro-Electro-Mechanical Systems). This sensor is manufactured by Bosch see - [Humidity Sensor BME280 | Bosch Sensortec](https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/)

This sensor has 3 inbuilt sensors, that provide data via an I2C interface.  The module identifies itself using a unique I2C bus address, so multiple devices can use the same bus, and the module will only respond when directly addressed with the correct address. The module then allows each individual register containing data temperature, humidity and pressure values  to be read using I2C addressing. This function is simplified by using a built in library driver, which takes care of the low level addressing and provide a high level application data interface. 

Internally this sensor has a **pressure** sensitive membrane connected to piezo resistive Wheatstone bridge.  So when pressure is applied, the piezo bridge is able to generate a voltage directly proportional to the pressure. See picture below

<img width="310" height="192" alt="(3)SensorExplanation" src="https://github.com/user-attachments/assets/2a6d66b7-61bf-4655-850e-23e552d2c2bb" />

Source - [How the BME280 Works | MEMS Overview](https://www.youtube.com/watch?v=e7m8_enxydY)

The **humidity** sensor works on the principle of capacitance which varies depending on humidity changes in the porous sensing material that is very sensitive to water. This change of capacitance can then be converted to a signal.

<img width="326" height="126" alt="(4)HumiditySensorExplanation" src="https://github.com/user-attachments/assets/19503cd9-8ada-4ac8-9e13-077959360a82" />

Finally the **temperature** measurement is performed by the sensitivity of a diode which conducts more when temperature increases, therefore changing its resistance. Using a voltage divider principle, then the output voltage will change directly proportionally to the temperature and therefore this signal can be measured. See below

<img width="198" height="254" alt="(5)VoltageDivider" src="https://github.com/user-attachments/assets/fa77c79a-862f-4a82-ae31-8ebed8e88383" />

## Software Implementation 
Version 2.3.0 of the Adafruit BME280 Library will be used, available from the library manager of the Arduino IDE.

This will use the following.

    #include <Adafruit_BME280.h>

 I will then create an instance of this to use in the program during initialization as below
 
    Adafruit_BME280 bme; //i2C

During the setup, we will check to confirm we are able to communicate with the BME_280 by calling a function to setup the address to be used for the sensor and check it is available at address 0x76 Hex, with a Boolean return response.

    bool communication = bme.begin(0x76);

We can then read each individual register by making the following function calls and storing the return value into a variable.

    Value = bme.readTemperature();

    Value = bme.readHumidity();

    Value = bme.readPressure();

## Light Sensor 
The light sensor uses a photodiode which generates an analog output voltage directly proportional to the number of photons (light) it receives. This analog voltage varies between 0v to 5V. Before it can be measured by the ADC, this voltage needs to be scaled to 3.3V as this is the maximum input voltage of the ADC. This is performed by a voltage divider, which uses ohm’s law and two resistors to generate a ratio which scales the input voltage so it can read by the ADC.

### Implementation 
For my design, then I will calculate using the following formula the resistor values required.

 Vout = Vin * (R2 / (R1 + R2)).
 
Vin = 5V, R1 = 5.1Kohm, R2 = 10Kohm – so Vout = (5 x 10000)/(5100 + 10000) = 3.3V

<img width="88" height="165" alt="(6)VoltageCalculation" src="https://github.com/user-attachments/assets/afafd30f-aed7-4a25-ae44-647d054be868" />

The ADC on the ESP32 is a 12bit ADC – this means it has 212 quantization steps – which is 4096. This means that that the voltage 0 to 3.3V can be expressed as an integer between 0 to 4096.

I will use the ADC1 - Channel 0 – which is Pin 36 for the input for the lightsensorpin.

This will read using the following function call:-

    lightsensor_adc_value = analogRead(lightsensorpin);

## Buzzer Module 
The buzzer uses a Piezo that converts an electrical signal into a sound wave. It needs a 5V voltage and output frequency to vibrate at a given frequency. The ESP32 output pins can be configured to generate a square wave output at a given frequency. This output signal can be provided to the buzzer module, which has a transistor to switch a 5V voltage and generate an audible output.

As described in experiment 5 - The Arduino has a built in function called Tone, which can be used to define a specific pin, the frequency and duration – which allows full control of the buzzer output.

This function requires just 3 parameters

•	pin – Output pin on which to generate the tone.

•	Frequency - Frequency of the tone in hertz. 

•	Duration - Duration of the tone in milliseconds.

e.g. 

tone (13,1200,500); - Will output on GPIO13, a 1200hz square wave for 500 milli seconds.

### Implementation

The Buzzer will be used in this project to generate two distinct outputs

1)	Normal operational - using a basic two-tone output each time the main loop has complete a full check of all input sensors. This will occur approximately once every 2 seconds.
   
2)	Alarm Output - When in Alarm mode, the buzzer will be used to play a tune, using a defined sequence of notes (frequencies) & durations – as it is very distinctive and demonstrates what is possible via coding. 
This is commonly available on the internet. I used the following site as inspiration. [Star Wars on a Buzzer | Arduino Project Hub ](https://projecthub.arduino.cc/HiHiHiHiiHiiIiH/star-wars-on-a-buzzer-e70139) 

## LCD Display 
The technical detail on how a 16x2 LCD display works is using a process to apply a voltage to dot (dot matrix) or segment which causes it to change its molecular alignment, affecting light polarization and making it either transparent or opaque. A display controller that is typically built into the display then converts a requested character, and drives the signals required to control the liquid crystals and hence is able to display a character on the screen.

As described in Experiment 5, a 16 x 2 LCD display can be driven by an I2C serial interface. This display has two rows of 16 characters, with a backlight to enable easy reading in low light conditions. This display can be driven via a driver function, that exposes easy interfaces to write data to this display.

<img width="386" height="147" alt="(7)LCDdisplay" src="https://github.com/user-attachments/assets/00650176-8069-43dc-b8b3-a14143bdd163" />

### Implementation 
The implementation in the final design is very similar to what was discussed in Experiment 5 – so please refer to this for more details.

The design uses a library function. This is defined by including this library.

    #include <LiquidCrystal_I2C.h>

An instance of the LCD object is set up based on I2C address 0x3F and set up for 16 x 2 operation using the following definition:-

    LCD_I2C lcd(0x3F,16,2);

Then following functions are implemented throughout the design.

•	lcd.backlight(); - Turns LCD Backlight ON

•	lcd.noBacklight();  - Turns LCD Backlight OFF

•	lcd.setCursor (0,0); - Sets the active character to a local on the 16 x 2 display

•	lcd.print() ; Function  to write data to the display from the active character location

•	lcd.clear();  Clears all data from the display

In order to reduce power consumption, then the backlight is turned off, until a button is pressed which turns on the backlight.

## Button 
This is the simplest component, which is a simple switch. When connected to an input with an applied voltage, then the digital input registers the switch pressed as digital logical high, and when not pressed a digital logical low.

## Implementation
Pin 2 on the ESP32 was defined as digital Input. A switch was connected to pin 2 and to the 3.3V rail. The code then checked the status by performing the following call.

    switch1_state = digitalRead(switch_input1);// read button input

  Whilst this worked, it was not reliable. There was a lot of false positives which appeared random. After a while it became clear that this was due to an incorrect design, as the voltage input was floating. It needed a resistor to pull the voltage to a known state when the button was not pressed. This was fixed by adding a 100K ohm resistor to ground to the input.

## Stepper Motor & Stepper Motor Driver
As described in Experiment 3, a stepper motor can be controlled by driving steps that control the coils in the more  in a specific sequence. As the coils require more than 500mA which cannot be performed directly by the ESP32 – then this function is performed by a driver circuit, such as the ULN2003 circuit that can provide the power from the 5v rail.

## Implementation

 During the experiment 3 it was demonstrated that the stepper could be rotated clockwise or anticlockwise, either continuously or by a discrete number of steps, that represent an angular degree.
 
For our implementation, it is necessary to drive the stepper to control the position of a solar panel, such that it is always facing the sun.  This requires both clockwise and anticlockwise control. 

As the design is remote controlled, then a web application was used to allow a user to define clockwise or anticlockwise direction, with a start / stop. 

Initially a LED was used to indicate direction, but this was removed in the final design as only necessary for debug.

The torque of the stepper motor appears sufficient to drive the rotation of the display directly.

A dedicated function called Onestep is called every 20ms to drive the stepper motor in the requested direction as follows. This gave the optimum speed and control in my tests.

    if (start == true){
 
    OneStep(step_direction);
    
    delay(20);
    }
The onestep function is responsible to follow sequence of 4 states, which is switched by case statement as follows: (Example Clockwise direction)

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
 
## Solar Charger with 5V output
This uses a 2000mA battery and 8W solar panel, which is connected to a power management module called BQ25185. This is manufactured by Adafruit
see [https://www.adafruit.com/product/6106](https://www.adafruit.com/product/6106)

<img width="332" height="265" alt="(8)BoostBoard" src="https://github.com/user-attachments/assets/30c52149-0fd1-437a-9ae2-0e01849bc44e" />

## Implementation
The battery output provides the current output of the battery, which indicates if it discharged or being charged. The nominal voltage is 3.7V when connected to a battery, but this can increase slightly when being charged, and can reduce to 2.5V when the battery is discharged. So knowing this information it can be useful to determine the state of charge of the battery.

See [Complete Guide to LiPo Battery Voltage](https://www.ufinebattery.com/blog/useful-overview-of-lipo-battery-voltage/)

<img width="470" height="353" alt="(9)VoltageDischargeGraph" src="https://github.com/user-attachments/assets/e7a0d98f-e7a4-4b7c-855b-d690df944a08" />

This signal is input into our ADC on Channel 1.  As this signal is greater than the 3.3V maximum that it requires to be scaled. 

For my design, then I will calculate using the following formula the resistor values required.

 Vout = Vin * (R2 / (R1 + R2)).
 
Vin = 5V, R1 = 5.1Kohm, R2 = 10Kohm – so Vout = (5 x 10000)/(5100 + 10000) = 3.3V

<img width="88" height="165" alt="(10)ResistorValueCalculation" src="https://github.com/user-attachments/assets/bc9843c2-be17-451f-a500-03e135ae8035" />

The ADC on the ESP32 is a 12bit ADC – this means it has 212 quantization steps – which is 4096. This means that that the battery voltage 0 to 3.3V can be expressed as an integer between 0 to 4096.

I will use the ADC1 - Channel 1 – which is Pin 34 for the input for the battery.

This will read using the following function call:-

batteryVoltageADC = analogRead(batterymonitorpin); // Read ADC value for Battery Voltage

 As this ADC value is reference to 3.3V, then it is necessary to scale it back to 5V, to provide the original voltage signal. This is done as below, which ADCRANGE = 4096 & MAXOUTPUT = 5   
 
batteryVoltage = (float)batteryVoltageADC/ADCRANGE*MAXOUTPUT; // Convert this to a voltage

## Challenges Overcome
Throughout the six different experiments, I have identified several challenges and improvements that I could make to the final design, based on my initial concept idea.

•	Increase width size of breadboard to easier access to both rows of Arduino.

•	Complexity to use 2 Stepper motors to drive solar panel not required. This would increase power demands significantly and  exceed capability of the power supply. Instead, I can fix one angle of the solar display to follow arc of sun.

•	Connecting a stepper motor output shaft to drive the display requires more mechanical engineering effort / capability , especially to overcome friction which would require a bearing– so only simple concept built. 
•	 Use of Simultaneous Access Point mode for WiFi allows ability to connect to an existing network router and allow remote WAN connectivity, whilst also allow ad-hoc direct connection to the stationary WiFi for configuration & local reading. 

•	The software design needs to be re architected to consider all of the peripherals and functions that will be used, so the main loop can run efficiently and there is sufficient bandwidth for each function to execute its purpose and provide real time information to the user.

•	The digital input signal from the button was randomly indicated it was pressed or released, when there was no actual interaction. The cause was identified that the GPIO needs either a pull UP or pull DOWN resistor to the input that the button is connected to avoid the condition when the button is not pressed it is  open circuit the voltage input to the digital input is floating. I choose a 100kOhm resistor to pull the input to 0V when not pressed, and when the button was pressed would be connected to 3.3V.

## Remaining Challenges
•	Setting the alarm threshold remotely via WiFi – as currently hardcoded. I believe I need to update the html code allow user to select a value and push this value using the POST method

•	Enabling Low Power modes of Arduino either based on time or an event

•	Logic to move solar panel based on combination of light sensor output and solar voltage rather than battery voltage.

•	The Light Sensor should be fitted to the solar panel, so it is measuring what the solar panel is observing.

## Critical Evaluation

This project has remained reasonably faithful to the initial project concept. It achieved the goal of being able to remotely monitor environmental values and communicate these whilst being powered independently. It underestimated the mechanical effort needed to move a solar panel with a motor and link this direction to the arc of the sun, although the basic concepts were exploreed in the experiments. The ESP32 was a very good platform to use and the experiements were able to demonstrate a wide variety of physical computing features, and throughout this process it enabled a lot of useful learning, especially when there were problems such as discovering I2C address or a button input randomly changing state. It was useful to build low level connectivity directly to the micorcontroller rather than rely on abstracted library functions - as this process generated a deeper understanding.
