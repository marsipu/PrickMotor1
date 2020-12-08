/*
 * Pinprick-Motor Control
 * MotorA - Vertical Movement Pinprick
 * MotorB - Horizontal Movement Stimulator ca. 0.3cm/Turn
 * D2 - Enable MotorA
 * D3 - Enable MotorB
 * D12 - MotorA-Position
 *
 * Trigger-Channels:
 * A0 - Trigger Ch6
 * A2 - Trigger Ch2
 * A3 - Trigger Ch3
 * A4 - Trigger Ch4
 * A5 - Trigger Ch5
 *
 * Outputs to Matlab
 * A1 Start MotorA
 * A2 Start MotorB
 * A1 + A2 End MotorA/End MotorB
 *
 * Trigger #32(6)(A0) - Reserved for Pinprick-Commands, no Command without it
 * Trigger #36(6+3)(A0+A3) - Start Trial
 * Trigger #40(6+4)(A0+A4) - MotorA goes to Zero-Position
 * Trigger #48(6+5)(A0+A5) - MotorB Reset Position
 *
 * To establish:
 * - Stop command (in delay)
 */


#include <Stepper.h>

const int stepsPerRevolution = 200; // How many steps are one round (Motor Specification)
const int minwalk = 100; // How many steps at least to be taken by MotorB

int Position = 0; // MotorB Position
int walk = 0;
int MotorAon = 0;
int rounds = 0;
const int maxPosition = 9 * stepsPerRevolution; // ca. 0.3cm/Turn, depends on hand length

//Initialize Motors with Stepper.h-Library
Stepper MotorA(stepsPerRevolution, 4, 5, 6, 7);
Stepper MotorB(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // put your setup code here, to run once:

  pinMode(2, OUTPUT); // enable MotorA
  pinMode(3, OUTPUT); // enable MotorB
  pinMode(A0, INPUT); // Pinprick-Command
  pinMode(A1, OUTPUT); // Empty Room Test
  pinMode(A2, OUTPUT); // Feedback for Matlab
  pinMode(A3, INPUT); // Start Trial
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

// Program 1: Single Trials
if(digitalRead(A0)==1 && digitalRead(A3)==1){
  digitalWrite(2, HIGH); // enable MotorA

  MotorA.setSpeed(50);

  while(digitalRead(12)==1) MotorA.step(1); // compensate resistance, start at zero
  MotorA.step(stepsPerRevolution); // negative Steps, because the rod lays on the tubes left side
  while(digitalRead(12)==1) MotorA.step(1); // compensate turn resistance, to do exactly one turn

  digitalWrite(2, LOW); // disable MotorA

  MotorB.setSpeed(200); // faster results in less torque

  digitalWrite(3, HIGH); // enable MotorB

  MotorB.step(stepsPerRevolution);
  digitalWrite(3, LOW); // disable MotorB

  delay(2000);
  }
}



if(digitalRead(A0)==1 && digitalRead(A4)==1 && MotorAon==0){ // get MotorA in Zero Position, Trigger #40
  while(digitalRead(12)==1){
    digitalWrite(2, HIGH); // enable MotorA
    MotorA.setSpeed(100);
    MotorA.step(1);
  }
  MotorAon = 1;
}

if(digitalRead(A0)==1 && digitalRead(A4)==1 && MotorAon==1){
  MotorAon = 0;
  digitalWrite(2, HIGH); // enable MotorA
  MotorA.step(stepsPerRevolution);
  digitalWrite(2, LOW); // disable MotorA
}


if(digitalRead(A0)==1 && digitalRead(A5)==1){
  Position = 0; // MotorB Reset Position, Trigger #48
  rounds = 0;
  MotorB.setSpeed(200);
  digitalWrite(3, HIGH); // enable MotorB
  MotorB.step(stepsPerRevolution);
  MotorB.step(-stepsPerRevolution);
  digitalWrite(3, LOW); // disable MotorB
}

}
