/*
 * Pinprick-Motor Control
 * MotorA - Vertical Movement Pinprick
 * MotorB - Horizontal Movement Stimulator ca. 0.3cm/Turn
 * D2 - Enable MotorA
 * D3 - Enable MotorB
 * D12 - MotorA-Position
 *
 * Trigger-Channels:
 * A0 - Trigger Ch6 Output
 * A2 - Trigger Ch2 Input
 * A3 - Trigger Ch3 Input
 * A4 - Trigger Ch4 Output
 * A5 - Trigger Ch5 Output
 *
 * Trigger #32(6)(A0) - Reserved for Pinprick-Commands, no Command without it
 * Trigger #40(6+4)(A0+A4) - Start Trial
 * Trigger #33(6+1)(A0+A1) - MotorA goes to Zero-Position
 * Trigger #48(6+5)(A0+A5) - MotorB Reset Position
 * Define Prickstim Trigger for MotorA/MotorB-Knob: 1975 in prickstim File
 */


#include <Stepper.h>
#include <HalfStepper.h>

const int One_Turn = 200; // How many steps are one round (Motor Specification)
const int minwalk = 200; // How many steps at least to be taken by MotorB
const int maxPosition = 9 * One_Turn; // ca. 0.3cm/Turn, 0.2s/Turn(300RPM)--> max. 1.8 s/2.7cm

int Position = 0; // MotorB Position
int walk = 0;
int ASpeed = 50;
int BSpeed = 300;
float c = 0.5; // Acceleration Factor MotorB

//Initialize Motors with Stepper.h-Library
Stepper MotorA(One_Turn, 4, 5, 6, 7);
Stepper MotorB(One_Turn, 8, 9, 10, 11);

void setup() {
  // put your setup code here, to run once:

  pinMode(2, OUTPUT); // enable MotorA
  pinMode(3, OUTPUT); // enable MotorB
  pinMode(A0, INPUT); // Pinprick-Command
  pinMode(A1, INPUT); // enable HalfStep
  pinMode(A2, OUTPUT); // Offset-Feedback
  pinMode(A3, OUTPUT); // Feedback Velo
  pinMode(A4, INPUT); // MotorA Zero-Position
  pinMode(A5, INPUT); // MotorB Reset Position

  // get MotorA in Zero Position
  while(digitalRead(12)==1){
    digitalWrite(2, HIGH); // enable MotorA
    MotorA.setSpeed(100);
    MotorA.step(1);
  }
  digitalWrite(2, LOW); // disable MotorA

  Serial.begin(9600);
}


void loop() {
  // put your main code here, to run repeatedly:

if(digitalRead(A0)==1 && digitalRead(A4)!=1 && digitalRead(A5)!=1){ //#32

    if(digitalRead(A1)==1){ //enable Half-Steps
      HalfStepper MotorA(One_Turn, 4, 5, 6, 7);

      delay(random(50,1000)); //1s

      MotorA.setSpeed(ASpeed);
      digitalWrite(A2, HIGH);
      delay(10);
      digitalWrite(A2, LOW);

      digitalWrite(2, HIGH); // enable MotorA
      while(digitalRead(12)==1) MotorA.step(1); // compensate resistance, assume pull-back(push would result in extra turn)
      /*while(c<1){
        MotorA.setSpeed(ASpeed*c);
        MotorA.step(One_Turn/10);
        c+=0.1;
      }*/
      MotorA.step(One_Turn); //0.6s
      /*while(c>0.5){
        MotorA.setSpeed(ASpeed*c);
        MotorA.step(One_Turn/10);
        c-=0.1;
      }*/
      digitalWrite(2, LOW); // disable MotorA

      delay(2000); //2s

      MotorA.setSpeed(ASpeed);
      digitalWrite(A3, HIGH);
      delay(10);
      digitalWrite(A3, LOW);

      digitalWrite(2, HIGH); // enable MotorA
      MotorA.step(One_Turn); //0.6s
      while(digitalRead(12)==1) MotorA.step(1); // compensate turn resistance, to do exactly one turn
      digitalWrite(2, LOW); // disable MotorA

      delay(1000); //1s
    }else{
      Stepper MotorA(One_Turn, 4, 5, 6, 7);
      delay(random(50,1000)); //1s

      MotorA.setSpeed(ASpeed);
      digitalWrite(A2, HIGH);
      delay(10);
      digitalWrite(A2, LOW);

      digitalWrite(2, HIGH); // enable MotorA
      while(digitalRead(12)==1) MotorA.step(1); // compensate resistance, assume pull-back(push would result in extra turn)
      MotorA.step(One_Turn/2); //0.6s

      digitalWrite(2, LOW); // disable MotorA

      delay(2000); //2s

      digitalWrite(A3, HIGH);
      delay(10);
      digitalWrite(A3, LOW);

      digitalWrite(2, HIGH); // enable MotorA
      MotorA.step(One_Turn/2); //0.6s
      while(digitalRead(12)==1) MotorA.step(1); // compensate turn resistance, to do exactly one turn
      digitalWrite(2, LOW); // disable MotorA

      delay(1000); //1s
    }
    walk = random(0-Position, maxPosition-Position); // Stimulator stays in defined Area(0-maxPosition), if MotorB Positon was reseted (e.g. to Knuckle-Line)
    while(abs(walk)<minwalk){ // walk has to exceed minwalk
      walk = random(0-Position, maxPosition-Position);
    }
    Position += walk;

    digitalWrite(3, HIGH); // enable MotorB

    while(c<1){
      MotorB.setSpeed(BSpeed*c);
      MotorB.step(walk/20);
      c+=0.1;
    }

    MotorB.setSpeed(BSpeed); // faster results in less torque
    MotorB.step(walk/2); // 0.9s.

    while(c>0.5){
      MotorB.setSpeed(BSpeed*c);
      MotorB.step(walk/20);
      c-=0.1;
    }

    digitalWrite(3, LOW); // disable MotorB

    // total ~8s, rating 5s
}


if(digitalRead(A0)==1 && digitalRead(A4)==1 && digitalRead(A5)!=1){ // get MotorA in Zero Position, Trigger #40
  HalfStepper MotorA(One_Turn, 4, 5, 6, 7);

  digitalWrite(2, HIGH); // enable MotorA
  MotorA.setSpeed(ASpeed);
  while(digitalRead(12)==1) MotorA.step(1); // compensate turn resistance, to do exactly one turn
  MotorA.step(One_Turn);
  digitalWrite(2, LOW);
  delay(2000);
  digitalWrite(2, HIGH);
  MotorA.step(One_Turn);
  while(digitalRead(12)==1) MotorA.step(1); // compensate turn resistance, to do exactly one turn
  digitalWrite(2, LOW); // disable MotorA
}


if(digitalRead(A0)==1 && digitalRead(A4)!=1 && digitalRead(A5)==1){ // #48 MotorB Reset Position
  Position = 0;
  MotorB.setSpeed(BSpeed);
  digitalWrite(3, HIGH); // enable MotorB
  MotorB.step(maxPosition);
  delay(200);
  MotorB.step(-maxPosition);
  digitalWrite(3, LOW); // disable MotorB
}

}
