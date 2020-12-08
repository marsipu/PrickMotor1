
#include <Arduino.h>
#include <ArdumotoStepper.h>

const int stepsPerRevolution = 200;

// Motor1(Ardumoto)
ArdumotoStepper myStepper(stepsPerRevolution);

// Motor2(DRV8825)
#include "DRV8825.h"
#define DIR 8
#define STEP 9
#define MODE0 10
#define MODE1 11
#define MODE2 12
#define ENABLE 13
#define MOTOR_STEPS 200
#define RPM 120
DRV8825 stepper(MOTOR_STEPS, DIR, STEP, ENABLE, MODE0, MODE1, MODE2);


void setup() {
  // put your setup code here, to run once:
pinMode(2, INPUT); // On switch
pinMode(5, INPUT); // Motor1 Starting_Point_Indicator
pinMode(6, INPUT); // Motor_Test-Switch
pinMode(8, OUTPUT); // Motor2 Direction
pinMode(9, OUTPUT); // Motor2 Step

while(digitalRead(5)==1){
  myStepper.setSpeed(150);
  myStepper.step(1);
}
Serial.begin(9600);
randomSeed(analogRead(0));

stepper.begin(RPM);
stepper.enable();

}


void loop() {
  // put your main code here, to run repeatedly:


while(digitalRead(6)){
  stepper.setMicrostep(1);
  stepper.rotate(360);     // forward revolution
  stepper.rotate(-360);    // reverse revolution
}

//Motor1 Pinprick-Stimulation
//ER
if(analogRead(A1)<500){

int ER_Rounds = 1;




    int counter = 0;
    while(counter<ER_Rounds && digitalRead(2)){
      myStepper.setSpeed(100);
      myStepper.step(200);
      delay(4000);
      delay(random(1000));
      counter++;

      if(counter=ER_Rounds-1){
        myStepper.setSpeed(0);
    }
   }

}



//Wind-Up
if(analogRead(A1)>500){

int WU_Rounds = 10;

    int counter = 0;
    while(counter<WU_Rounds && digitalRead(2)){

      myStepper.setSpeed(100);
      myStepper.step(200);
      delay(random(50));
      delay(random(200));
      counter++;


      if(counter==WU_Rounds-1){
        int counter = 0;
        delay(10000);
      }
    }


}


}
