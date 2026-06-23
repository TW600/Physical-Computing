# Experiment 4 - Analog to Digital Convertor 

This experiment is interested to know how an analogue signal can be measured using a digital device.  Often sensor devices don’t output register values, as was the case for the I2C sensor, but instead an analogue voltage that is proportional to the level of the measured quantity.

For our experiment I need to measure the light level to understand where the solar panel should be directed for optimum efficiency.  The light sensor using a photo diode/triode, which generates a voltage output signal depending on received light levels.

The sensor which I have chosen to integrate is the Grove Light Sensor 1.2 – this is a very cheap sensor (£2)  and is available from  [Grove - Light Sensor v1.2 | The Pi Hut ](https://thepihut.com/products/grove-light-sensor-v1-2?variant=20063085396030&country=GB&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&gad_source=1&gad_campaignid=22549809780&gclid=EAIaIQobChMIwdvuyejAjQMVkJlQBh1WYgDDEAQYASABEgKOuvD_BwE)

<img width="270" height="203" alt="(1)Sensor" src="https://github.com/user-attachments/assets/1bb00e4a-658b-47eb-83f9-7132124d1488" />

This sensor requires 3 connections

•	VCC – (3V- 5V)

•	GND – 0V

•	SIGNAL – (0 – VCC voltage)

To measure this signal, then it is required to be connected to the Analogue to Digital converter (ADC) on the ESP32. 

The ESP32 has two separate 12bit ADCs, known as ADC1 and ADC2. 

ADC 1 has 8 channels, and ADC2 has 10 channels.  Normally both ADC’s can be used simultaneously – but each channel can only be used sequentially.

However, the datasheet explains that if WiFI is being used, then ADC2 is not available [Analog to Digital Converter - ESP32](https://docs.espressif.com/projects/esp-idf/en/v4.2/esp32/api-reference/peripherals/adc.html)

Therefore for this experiment only ADC1 will be used.

The lightsensor is connected to GPIO36Pin 35, which is ADC1_channel0 (see below)

<img width="458" height="141" alt="(2)GPIOpinDiagram" src="https://github.com/user-attachments/assets/3355d5ca-72ef-4810-86fb-08e9347c2113" />


To read the value of the ADC, there is a built in function on the ESP32 called AnalogRead(). This just request the pin to be defined that is being read – see [analogRead() | Arduino Documentation](https://docs.arduino.cc/language-reference/en/functions/analog-io/analogRead/)

The following line of code is then used to read the ADC value.

    lightsensor_adc_value = analogRead(lightsensorpin); // Read ADC Value for Light sensor
    
This value was the read every 1 second, and the value printed out on a Serial.println – so it could be easily observed on the serial monitor.

## Initial problem

As the ADC is 12bit – then I am expecting 212 = 4096 as the maximum value of the ADC, so effectively my signal will use the range 0 – 4096.

When I connected the light sensor to Vin to 3.3V and connected the signal to the ADC channel, and then pointed the light sensor to strongest  light source I had, then then maximum value I could read was 2038, and it required quite large changes in light to make a difference in the ADC value. 

I then temporarily changed the Vin from 3.3V to 5V – and the ADC range increased to 4096, until I remembered the maximum voltage on the ADC pin was 3.3V.

This indicated that the output signal of the lightsensor was attenuated, or the ADC was attenuating the signal before reading.

To solve the problem, then I needed to connect the lightsensor Vin to 5V and create a voltage divider using 2 resistors as calculated below to reduce the signal voltage to 3.3V 

Vout = Vin * (R2 / (R1 + R2)).

Vin = 5V, R1 = 5.1Kohm, R2 = 10Kohm – so Vout = (5 x 10000)/(5100 + 10000) = 3.3V

<img width="88" height="165" alt="(3)Voltage" src="https://github.com/user-attachments/assets/b4832344-5651-4873-8ef2-0d0101a417f9" />

When this was implemented, then the ADC for the light sensor when read by the AnalogRead() function provided a full range depending on light levels between 0 to 4096.

I then read the documentation in more detail regarding the ADC function on the ESP32, and understand that the ADC has built in attenuation options. These can be changed using the analogReference()  function see - [- analogReference() | Arduino Documentation](https://docs.arduino.cc/language-reference/en/functions/analog-io/analogReference/). By default, the attenuation assumes a 5V input level, so scales down the signal. It is possible to change this to 3.3V. Additionally, there are functions on the ESP32 to perform this directly.

The below example is also possible, which sets the attenuation to 0dB

    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);

As I had already connected the Vin to 5V and created a voltage divider, then I left the solution as is, but at least I know understand what happened.

