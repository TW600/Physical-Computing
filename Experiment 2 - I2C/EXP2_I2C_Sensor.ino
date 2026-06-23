#include <Wire.h>
#include <Adafruit_BME280.h>
 
Adafruit_BME280 bme; //i2C

float temp, pressure, humidity;

void setup() {

  Serial.begin(115200); 
  
  bool serial_communication = bme.begin(0x76); // The datasheet says default addressis 0x76 for BME280

   if (!serial_communication) {

     Serial.println("Could not find a valid BME280 sensor");

     Serial.println("check wiring, address, sensor ID!");

     Serial.print("SensorID was: 0x");

     Serial.println(bme.sensorID(), 16);

     Serial.println("ID of 0xFF probably means a bad address\n");

     while (true) { }; // Stop in endless loop

     delay(10);

    }

 
}

void loop() {

// Means that serial communication was OK, so read the values every 1 second

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

}