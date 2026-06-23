#include <Wire.h>

// Stepper Motor definitions
#define STEPPER_PIN_1 12
#define STEPPER_PIN_2 14
#define STEPPER_PIN_3 27
#define STEPPER_PIN_4 26

int step_count =0;
int stepnumber =0;

int limit =0;

//28BYJ-48 Stepper motor datasheet has 5.625 degrees per step. So 360 degrees requires 360/5.625 = 64 steps for one rotation
// The motor has a gearbox with a 1:32 reduction - so external shaft rotation is 64 x 32 = 2048 steps
// To calculate steps per degree then 2048/360 = 5.6888. so 90 degree - 5.6888 x 90 = 512 steps

#define FULL_ROTATION 2048;
#define NINETY_DEGREES 512;

bool clockwise_direction = true;
bool drive = true;

void setup() {

  Serial.begin(115200);   

  pinMode(STEPPER_PIN_1, OUTPUT);      // set the IO pin as output for Stepper motor
  pinMode(STEPPER_PIN_2, OUTPUT);      // set the IO pin as output for Stepper motor
  pinMode(STEPPER_PIN_3, OUTPUT);      // set the IO pin as output for Stepper motor
  pinMode(STEPPER_PIN_4, OUTPUT);      // set the IO pin as output for Stepper motor
  
  step_count = 0; // initialise step count
  drive = true;
  limit = NINETY_DEGREES;

}
 
void loop() {

  while (drive == true){
    OneStep(clockwise_direction);
    delay(10); //10ms delay between steps (fast)
    step_count++;
    if (step_count == limit)
    {
      drive = false; // stop after reaching limit of  degrees
      delay(5000); //pause 5 seconds
    }
  }

  if (clockwise_direction == false)  {//change direction at each limit
    clockwise_direction = true;
     }
  else{
    clockwise_direction = false;
  }//
  
  drive = true;//resume drive 
  step_count =0;

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
