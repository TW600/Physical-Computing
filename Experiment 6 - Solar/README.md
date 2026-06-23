# Experiment 6 - Power Management 

The ESP32 can be powered independently from a USB connection and can operate in both sleep and active modes to minimise consumption. The ESP32 can be powered externally by a 5V VIN connection and can generate both 5.0V and 3.3V levels to support all functions at the power levels required. 

This experiment intends to understand how this is can be achieved, and whether a solution can be designed to enable long term independent operation suitable for monitoring solution.

The intention is to use a combination of a Solar Panel, rechargeable battery and low power modes to prove that the solution can operate without connected power for more than 24 hours.

The starting point is to understand the energy requirements of the ESP32WROOM.

According to the datasheet - [- esp32_datasheet_en.pdf](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) the modem has various low power modes with reduced current consumptions.

<img width="602" height="326" alt="(1)Datasheet" src="https://github.com/user-attachments/assets/e90fe25a-097c-4ff0-bde7-a05f4abbd061" />

For the active mode when the modem is awake and WiFI connectivity available, then the typical power consumption in idle active is 100mA

<img width="602" height="193" alt="(2)ActiveModeDatasheet" src="https://github.com/user-attachments/assets/4818eaa4-b2aa-460a-a1ce-885b688c4912" />

For the purposes of this test – I will assume that 100mA is the average typical current consumption, but also ensure that the worst case current consumption of 240mA can be supported. This is an essential requirement.

The ESP32 also need to be able to drive various peripherals at 5V, with the most demanding being the stepper motor driver ([ULN200xx.fm](https://www.st.com/resource/en/datasheet/uln2001.pdf)) with demands upto 500mA – so it is essential the input voltage is 5V, rather than 3.3V with the ability to provide 500mA. So this means a solution able to provide 740mA at 5V is an essential requirement.

Finally – I want to ensure that the solution can operate independently for 24hrs and can monitor it power supply condition so that it can take action in case it need to switch to a lower power mode.  So the ability to monitor the power supply is essential.

## Choice of Ada Fruit 5V buck Boost

An off the shelf solution typically used is the Adafruit solar charger with 5V output  

Reference [Adafruit bq25185 USB / DC / Solar Charger with 5V Boost Board : ID 6106 : Adafruit Industries, Unique & fun DIY electronics and kits](https://www.adafruit.com/product/6106)

<img width="518" height="356" alt="(3)BoostBoard" src="https://github.com/user-attachments/assets/0570bc46-49ff-4858-9e55-01168feb7113" />

This solution provides a 5V output with a 1000mA (1A) output – which ensures it is easily able to meet the 740mA requirement of the ESP32 solution, and also power external peripherals requirements.
Finaly this solution provides a battery monitor output – which allows the status of the solution to be monitored.

## Solar Energy

As the solution requires a minimum of 740mA @5V – this means an external supply of minimum  3.7W is needed to ensure there is sufficient power to provide power. 

There are various low cost solar panel solutions that provide a standard 5V output. An 8W output solution was chosen to ensure there was sufficient surplus energy to provide storage charge as well as providing direct energy. 

The following solar panel was chosen, as it is a popular choice of outside security cameras with an IP65 rating. 

[8W Solar Panel for Outdoor Security Camera, 5V Micro USB& USB-C Solar Panel Charger ，Fits Eufy/Wyze/Reolink DC 5V Output, IP65/ 360° Adjustable, 9.8ft Cable, Fast Charging : Amazon.co.uk: Business, Industry & Science](https://www.amazon.co.uk/dp/B0DHXS69L1?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1)

<img width="286" height="242" alt="(4)SolarPanelCharger" src="https://github.com/user-attachments/assets/d37d4e0c-3cb5-4d2f-bd8f-1c57615a7973" />

<img width="602" height="492" alt="(5)SolarPanelDatasheet" src="https://github.com/user-attachments/assets/9ab16a20-ab74-4941-af39-ecf9cf5ec355" />

## Rechargeable Battery

The most important component of externally powered solution is the battery. This needs to be rechargeable and have sufficient energy to be able to power the solution for sufficient time to allow uninterrupted operation.

If the assumption is that average power consumption is 100mA and the solar panel will only be able to provide energy for 8hrs a day, then this means that the solution will need to be able to operate on battery for 16hrs at 100mA – meaning 1600mAh capacity is needed.

Therefore a minimum 2000mAh capacity is desirable, using Lithium Polymer technology to enable recharging.  A typical solution can be found from [2000mAh 3.7V LiPo Battery | The Pi Hut](https://thepihut.com/products/2000mah-3-7v-lipo-battery?variant=42143258050755&country=GB&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&gad_source=1&gad_campaignid=11673057096&gclid=EAIaIQobChMI78md5fi8jQMVLJVQBh3tiAJnEAQYAiABEgK0RvD_BwE)

<img width="421" height="298" alt="(6)Battery" src="https://github.com/user-attachments/assets/fe50d74a-41df-4095-8437-79cc1f358a2e" />

## ESP32 - Solution Implementation

The 5V output of the Solar panel is connected to the Vin and Gnd pins of the Adafruit BQ25185 as per the schematic diagram. This will automatically charge the 3.7V battery if there is excess power and provide power to the 5V buck boost circuit. 

The 2000mAh battery is connected to the battery terminal. An orange LED indicates if this is being charged and green LED if battery is charged.

The 5V output of the Adafruit BS25185 is then connected to the ESP32 5V supply. This supply allows the ESP32 to generate necessary 5V an 3.3V rails for its functions.

Finally the analog Battery Output pin of the BQ25815 representing the current voltage of the 3.7V battery is connected to pin 34, which is a 12bit ADC channel on the ESP32. It is essential to connect this  via a voltage  divider circuit, as the voltage exceeds the max 3.3V voltage of the ESP32. 

The calculation is very simple, as can be seen below ( reference - [Voltage Divider Calculator](https://ohmslawcalculator.com/voltage-divider-calculator)) – and just requires a 10K ohm and 5.1kohm resistor.

<img width="384" height="282" alt="(7)VoltageCalculation" src="https://github.com/user-attachments/assets/c1eaa24c-7cb3-49e2-ab41-0453883142c2" />

The ADC input on the ESP32 can then be scaled to reflect this and output a measured voltage that represents the actual voltage.

This voltage is sampled every 1 second by the firmware and update on a LCD display to show the user the current voltage of the battery.

