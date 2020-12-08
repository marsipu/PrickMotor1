#include <ArdumotoStepper.h>

const int stepsPerRevolution = 200;

// Motor1(Ardumoto)
ArdumotoStepper myStepper(stepsPerRevolution);


void setup() {
  // put your setup code here, to run once:
pinMode(2, INPUT); // On switch
pinMode(5, INPUT); // Motor1 Starting_Point_Indicator

while(digitalRead(5)==1){
  myStepper.setSpeed(150);
  myStepper.step(1);
}
Serial.begin(9600);
randomSeed(analogRead(0));

}


void loop() {
  // put your main code here, to run repeatedly:

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
