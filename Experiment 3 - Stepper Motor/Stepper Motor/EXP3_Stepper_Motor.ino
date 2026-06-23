#include <Wire.h>

// Stepper Motor definitions
#define STEPPER_PIN_1 12
#define STEPPER_PIN_2 14
#define STEPPER_PIN_3 27
#define STEPPER_PIN_4 26

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds
const long timeoutTime = 10000; // change state every 10 seconds

int stepnumber = 0;
int direction_change = 0;
bool clockwise_direction = true;
bool slow = false;

void setup() {

  Serial.begin(115200);   

  pinMode(STEPPER_PIN_1, OUTPUT);      // set the IO pin as output for Stepper motor
  pinMode(STEPPER_PIN_2, OUTPUT);      // set the IO pin as output for Stepper motor
  pinMode(STEPPER_PIN_3, OUTPUT);      // set the IO pin as output for Stepper motor
  pinMode(STEPPER_PIN_4, OUTPUT);      // set the IO pin as output for Stepper motor

}

 

void loop() {
   
currentTime = millis(); // update time
if (currentTime - previousTime <= timeoutTime) // check if a timeout to change fast/slow, clockwise/anticlockwise
{
  if (slow == true){
    OneStep(clockwise_direction);
    delay(200);// 200ms delay between steps (slow)
  }
  else{
    OneStep(clockwise_direction);
    delay(50); //50ms delay between steps (fast)
  }
}
else
{
  previousTime = currentTime; //reset event time
  
  if (slow == false) // swap speed
  {
    slow = true;
  }
  else
  {
    slow = false;
  }
  
  direction_change++;

  if (direction_change > 2) // change direction after change of both speeds
  {
    if (clockwise_direction == false)
    {
      clockwise_direction = true;
    }
    else
    {
      clockwise_direction = false;
    }
    direction_change =0 ; // reset direction counter
  }

}

}

 

void OneStep (bool dir){

    if (dir){ //Clockwise direction

      switch(stepnumber){ //drives each coil sequentially clockwise

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

    else{//anticlockwise

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

if (stepnumber>3) //reset stepper motor cycle to repeat
{
  stepnumber =0;
}

}
