# Experiment 2- I2C Digital Sensor 

Experiment 2 – I2C Sensor
This experiment is interested to know how an I2C device can be connected to our project to enable data to be read related to a sensor device.  This is an interesting topic, as there are a huge number of different peripheral computing devices that provide connectivity, and they have various methods of communicating. 

Fundamentally there are two main communication methods.

o	Serial – This is where data is sent sequentially over a single line

o	Parallel- This is where data is sent simultaneously over multiple parallel lines

The key advantages are that parallel communication is significantly faster but is more complex & expensive, whereas serial communication is slower, ideal for longer communication distances and cheaper.

For industrial sensors – then serial communication appears an appropriate method for communication. However within this set, there are different serial communication protocols

•	Asynchronous – Where data is sent in separate bytes with start and stop bits between to allow receivers to process between bytes

•	Synchronous – Where all data is synchronized between sender and receiver with  a clock signal and sent in a continuous stream.

The I2C protocol is a Synchronous protocol – which is short for Inter -Integrated Circuit. This operates using two wires, 

•	SCL – Serial Clock Line

•	SDA  - Serial Data Line

This protocol supports a multi-master and multi-slave configuration, which allows multiple devices to share a single common bus.

<img width="450" height="217" alt="(1)MasterSlaveConfig" src="https://github.com/user-attachments/assets/5ddbb19c-4c37-49bf-a344-a5920985c7c4" />

For more information on the I2C communication protocol – See [Basics of the I2C Communication Protocol](https://www.circuitbasics.com/basics-of-the-i2c-communication-protocol/)

For my experiment – then I chose a I2C Sensor device that provides environment data, including Temperature, Humidity & pressure. These are low cost devices and very reliable.  One of the most populate devices for the Arduino is the BME280 sensor. This sensor device is based on a sensor from Bosch, but is integrated onto a small PCB that enables it to be easily fitted to a breadboard design.
I purchased this sensor from Amazon - [GY-BME280 I2C](https://www.amazon.co.uk/dp/B07D8T4HP6?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1)

<img width="474" height="248" alt="(2)I2Csensor" src="https://github.com/user-attachments/assets/d083839f-0ef7-4308-8538-722d2e009679" />

This sensor is really easy to connect to the Arduino.

Vin – 3.3V
 
Gnd – Gnd

SCL – SCL (pin 22)

SDA – SDA ( pin 21)

It is then necessary to install an I2C driver to read the BME280. Within the Arduino IDE library manager you can find a suitable library – I used the Adafruit BME280, version 2.3.0

<img width="312" height="422" alt="(3)LibraryManager" src="https://github.com/user-attachments/assets/78a1a150-35d7-4c62-8d67-4d6fc9863a9e" />

Once installed, then include the library.

    #include <Adafruit_BME280.h>
    
It is then necessary to declare an instance – which is done using the following:-

Adafruit_BME280 bme; // My i2C BME280

Information about the driver and methods supported are available here  - [Adafruit BME280 Library](https://github.com/adafruit/Adafruit_BME280_Library/blob/master/README.md)

## My 1st problem to overcome. 

Then during our setup() function, we can connect to the BME280 by using the bme.begin() function. This requires the I2C address of the sensor to communicate with.  When I first installed the device, I did not know the address.  

I solved this problem by using a brute force method, and scanning all possible addresses from 0x00 to 0xFF, and checking if I got a response. I developed small piece of software  (which is in the Github repository) that simply iterated each address, until there was a successful response and it printed the address to the Serial monitor.

Using this approach, then I discovered that the address used by the BME_280 was 0x76 hex.

To initiate communication call this function as follows:-

    bool communication = bme.begin(0x76);

We can then read each individual register by making the following function calls and storing the return value into a variable.

    Value = bme.readTemperature();

    Value = bme.readHumidity();

    Value = bme.readPressure();

These values are read every 1 second and displayed on to the Serial Monitor using Serial.println calls.
By touching the sensor with your finger or breathing on the sensor – it is extremely sensitive and will record the change in temperature, pressure and humidity.
