#  Experiment 1 - Remote Communication using WiFi

## Background 
The ability to communicate information between multiple physical devices which can then be used to make an informed decision and then take an action, is one of the most important use cases for almost all computing devices. Often computing devices may be located in areas where it is not easy or cost effective to create a physical connection – and so a wireless connection is preferred. When choosing a protocol for wireless communication, it is important that these connections are standards based to allow easy interoperability between devices and are robust & reliable to enable secure communication over an internet connection. The WiFi standard, based on IEE802.11 is a great example of such a protocol. 
 I want to understand how easy it is to build a remote connection using Wifi on a low cost physical computing device such as an Arduino.  Therefore this experiment sets out to explore what is involved to establish such a connection, and what can be performed.
 
 
 ## Description 
 The Arduino is chosen as a suitable low cost & low power microcontroller platform that has multiple variants with built in Wifi module. This platform has variants with an extensive set of inputs/outs that enable sensors to be connected and output to be controled. Importantly all variants support a flexible integrated development environment (IDE) and an extensive open source development library – which is ideal for development.
 
 
### Hardware
 For this project – the Arduino dev kit based on the ESP32 WROOM R4 was chosen specifically for its wireless connectivity. 
These devkits are readily available and  can be purchased for less than £10 - [ESP32 WROOM-32 Amazon](https://www.amazon.co.uk/ESP32-WROOM-32-Development-Board-Bluetooth/dp/B0DJPZHZ1X/ref=sr_1_1_sspa). They are ideal for initial breadboard development, and allow easy future transition to a PCB development to reduce size and cost when only the module is required.

<img width="552" height="303" alt="(1)ESP32" src="https://github.com/user-attachments/assets/5a51ca3f-c088-417e-b86f-127b5dd01f43" />


The module is developed by Espressif, who provide a comprehensive datasheet for this module [esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf)

The Wifi connectivity of the module is based on operating on the 2.4Ghz channel, and it supports 802.11 b/g/n. The module has a Tx power of 20dBm and sensitivity of -91dBm so should reasonably expect to have an operating range of up to 100m in free space. 
For the purposes of this experiment the fastest supported Wifi connection protocol will be used which is IEE802.11n. This claims up to 150Mbs


### Software
The Arduino IDE is used for this experiment. This can be downloaded from [Software | Arduino](https://www.arduino.cc/en/software/).
For the purposes of this experiment version 2.3.6 was used on a Windows 10 environment.

<img width="602" height="326" alt="(2)ArduinoCode" src="https://github.com/user-attachments/assets/9708a200-8ae4-4089-93ee-769fdddf633c" />

Once installed, you will notice that there are two functions pre-defined.
•	setup ()
•	loop()
The setup function is typically used for all configuration activities that initialises the startup operation of your functions. This is only called once, when the Arduino starts after a reset.
The loop function is used for the main function. This function is continuously called by the Arduino and is used to execute the operational code. 


### WiFi Implementation
The module support a wide variety of libraries designed to make using the ESP32 easy to implement. 
These libraries are fully documented on the Arduino documentation website – available here [WiFi | Arduino Documentation](https://docs.arduino.cc/libraries/wifi/) 
and the API is described here [Wi-Fi Overview | Arduino Documentation](https://docs.arduino.cc/language-reference/en/functions/wifi/overview/)

Normally the library is installed separately, but for the ESP32WROOM it has the library already embedded in its core.

To use this library, it is necessary to define the following header in your code

// Load Wi-Fi library

#include <WiFi.h>

There are three  fundamental modes of operation that the Wifi module can support. These are known as Access Point ( AP) and Station Modem (STA).
In this experiment we will test both to understand the key differences.


### 1. Access Point (AP) Only 
In this configuration, the Arduino acts as a wireless access point and creates its own dedicated network. Devices can connect directly to this network.


### 2. Station Mode (STA) Only
In this configuration, the Arduino allows access to an existing network, typically as wireless router. This allows the devices to join an existing Wifi network, and allows the possibility to be connected externally to a WAN connection if supported by the router.


### 3. Simultaneous Mode Access Point & Station Mode (AP_STA)
In this configuration, the Arduino allow connection to an existing Wi-Fi network while simultaneously creating its own network for other devices to connect. Useful when you need to use both the Arduino’s internet connection and a local network for other devices. 


