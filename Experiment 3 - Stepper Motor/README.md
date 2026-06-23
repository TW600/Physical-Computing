# Experiment 3 - Stepper Motor Control 

Stepper motors allow precise control over angular position, speed, and direction. In this experiment, we control a 28BYJ-48 stepper motor using an ULN2003 driver board interfaced with an ESP32.

<img width="161" height="179" alt="(1)StepperMotor" src="https://github.com/user-attachments/assets/f200e184-41b7-437a-b1d7-add3dec32a18" />

## This experiment is divided into two parts:

•	Part A: Speed and direction variation over time. [Stepper Motor Folder](https://github.com/BSU-tangible-computing/physical-computing-24-25-TW6000/tree/4c5a4cba3b600f7a17cb2d8aefaa65dafd5f775b/Experiment%203%20-%20Stepper%20Motor/Stepper%20Motor)

•	Part B: Precision angle control (e.g. 90° rotation). [Stepper Motor Precision Folder](https://github.com/BSU-tangible-computing/physical-computing-24-25-TW6000/tree/06ba31058258825d46989c254059b754a0bf5f29/Experiment%203%20-%20Stepper%20Motor/Stepper%20Motor%20Precision)

## Hardware 

•	ESP32 Development Board

•	28BYJ-48 Stepper Motor

•	ULN2003 Stepper Motor Driver Board

•	Jumper Wires & Breadboard

## Stepper Driver
It is necessary to use a Stepper motor driver board in order to provide the additional current needed to drive the coils. The coils typically require more than 500mA at 5V which cannot be provided directly by the ESP32. – hence it needs additional drive.  This is provided by the ULN2003 board that has a set of Darlington drivers for each coil output capable of providing high power.

## Stepper Motor
The stepper motor used is a 28BYJ-48, which is designed to operate at 5V DC.  As can be seen from the datasheet – [28BYJ-48 Stepper Datasheet ](https://www.mouser.com/datasheet/2/758/stepd-01-data-sheet-1143075.pdf?srsltid=AfmBOopSQ1kJH0FdYJ8VvqQWHetUIpIi5JmdRqsTcpSa46o98_n3KnY8) – the has 2 coils with a common centre tap. This splits the coil in two and provides a total of 4 phases that can be separately driven.

<img width="368" height="214" alt="(2)StepperMotorPhasesDiagram" src="https://github.com/user-attachments/assets/d6654eb9-1e50-4034-aed7-6031660dd029" />

Each driven phase or step  of the motor moves the shaft 11.25 degrees, so a complete rotation of motor requires 360/11.25 = 32 steps. In order to increase torque the motor also has a gearbox , this can be clearly shown in the picture below courtesy of [In-Depth: Control 28BYJ-48 Stepper Motor with ULN2003 Driver & Arduino](https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/)

<img width="483" height="287" alt="(3)StepperMotorGearRatios" src="https://github.com/user-attachments/assets/54cec170-bd4c-4ab0-b8ae-5fc03821317f" />

As can be seen the Gearbox has a ratio of 64:1 and the motor requires 32 steps for 1 rotation – so a total of 32 x 64 = 2048 steps are required for one full rotation.

## How to drive stepper motor with Arduino (ESP32)
The are various methods to drive a stepper motor in the Arduino.

The Arduino IDE makes it very easy to drive a stepper motor, and has a built in library function called “Stepper.h” which is fully defined in the Arduino documentation [Stepper | Arduino Documentation](https://docs.arduino.cc/libraries/stepper/)

This is very easiest method  to use, and only requires a simple set up. The below code shows examples of how easy this is to do. 

    // Pins entered in sequence IN1-IN2-IN3 IN4 for proper step sequence
    Steps_for_Revolution = 2048;
    Stepper myStepper = Stepper(Steps_for_Revolution, 12, 14, 27, 26);

Then to control speed or rotation, 

    // Rotate Clockwise slowly at 5 RPM
     myStepper.setSpeed(5);
    myStepper.step(stepsPerRevolution);
 
    // Rotate Anticlockwise quickly at 10 RPM
    myStepper.setSpeed(10);
    myStepper.step(-stepsPerRevolution);
    
The method I chose for this experiment did not use the driver, and instead I used the GPIO controls to directly drive the stepper motor using my own function, which is useful if you want to fully understand how to drive a stepper and have maximum control.

A dedicated function called onestep() was created, and this function called by our loop(). Then we can control the speed, by defining the delay between each step and the direction using a book to define clockwise[true/false]

A delay of 20ms between each step x 2048 – means it will take 40 seconds for one revolution, which was a suitable slow speed control via a user interface.

     if (start == true){
    OneStep(step_direction);
    delay(20);
    }

Using the above approach, the PART A – Speed and direction control can easily be observed.
If we need to increase speed, then the delay is simply reduced, or direction is either 

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


The PART B of the experiment was to use the step function to define a specific angular control.

For this approach, the onestep () function is reused and simply counts the number of steps and compares this against a predefined step count. For example, 90 degrees is 2048 steps /360 * 90 = 512 steps.

A separate test function was generated to demonstrate this, and show this with a pencil on the stepper motor shaft

