# Remote Control Monitoring Station 
The usage of remote-controlled monitoring is widespread within society and has many diverse applications. These include environmental (River levels & sewage), Medical (remote patient monitoring), Traffic (Flow & pollution), Retail (food, footfall ), security (buildings) and more.

For my project I want to envisage how the core components of a typical remote controlled monitoring solution play a critical role within a physical computing system. These functions enable the  ability to automatically read sensor data,  establish communication and then take a form of action. This systems are often located in difficult to access locations, so need to be autonomous. This typically means that the project has requirements to be powered independently and be able to communicate wirelessly.

<img width="329" height="185" alt="image1" src="https://github.com/user-attachments/assets/a5731188-d213-4f62-bc21-bac9460608f1" />
<img width="209" height="186" alt="image2" src="https://github.com/user-attachments/assets/3d4c9187-4218-4895-b6c9-8171e3706c03" />
<img width="325" height="183" alt="image3" src="https://github.com/user-attachments/assets/3561fc28-302e-4019-801b-afe9d27b8b57" />
<img width="327" height="245" alt="image4" src="https://github.com/user-attachments/assets/d0f37689-ffe2-455c-b77a-e8d79ba7a402" />


## Outline
This project will evaluate a set of physical computing technologies and aims to build a modular platform that is able to demonstrate the core component of a remote-controlled monitoring station. 

The concept is to demonstrate how a user, such as a quality engineer at a company, can monitor critical parameters and receive real time information.  For this project a typical laptop will be used as the local monitor, and it will use a Wi-Fi connection to the remote monitoring station. 

The requirement of the remote monitoring station are to read data from a variety of different analogue inputs which are connected to sensor devices and process this data and communicate. The remote monitoring station will also monitor for any alarms event, such as tamper and communicate immediately as well as monitor its own status and ensure it has sufficient energy to perform its function. To do this, it will generate energy using a solar panel and charge a rechargeable battery. This requires an energy controller to ensure the battery can be charged at the correct voltage and discharged reliably . For the solar panel to charge effectively and generate maximum power will need to follow the arc of the Sun in the sky.  This will require the use of two servo motors to control X & Y position of the solar panel. 

It is proposed that the following experiments are performed:-

**Experiment 1** - Determine if  a low cost ESP32 device which has built-in Wi-Fi capability can act as a system controller.  Understand how it is able to join a Wi-Fi at work and then be able to communicate data. This data would need to be able to be received by a PC and a small program used to visualise any data received.

**Experiment 2** - Determine how the system controller can receive multiple sensor inputs. The sensor input will need utilise the ADC capability of the micro to monitor temperature,  light levels and humidity.  This will require interfacing two off the shelf sensor devices and signal conditioning of the signals, so it is suitable to be processed.

**Experiment 3** - Determine how to generate maximum power from a solar panel by keeping it directed at the sun, and using two servos to control the X and the Y direction which use a motor controller that is able to be directly by the  system controller

**Experiment 4** - Determine how a system controller can manage the charging of the 12V battery and monitor the remaining capacity.

**Experiment 5** - Determine how a simple user interface can be connected to a controller to provide basic status information of the system controller 

**Experiment 6** - Determine how a digital event can trigger an alarm, and result in the system reacting immediately and communicate status data.

## Remote Controlled Monitoring Station Vision 
<img width="997" height="236" alt="(1)vision" src="https://github.com/user-attachments/assets/ed899bde-c0cf-4b46-b9b2-728ae1905d2a" />
<img width="653" height="492" alt="(2)vision" src="https://github.com/user-attachments/assets/888e308c-623c-44ed-803f-3a0e0543c620" />
<img width="1600" height="1091" alt="(3)vision" src="https://github.com/user-attachments/assets/2d9a48d5-5aac-4f10-b8de-fc721ad88aca" />
Link to Espressif 32 - [esp32_devkitC_v4_pinlayout.png (1600×1091)](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/_images/esp32_devkitC_v4_pinlayout.png)



