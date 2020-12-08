/*
 * Pinprick-Motor Control
 * MotorA - Vertical Movement Pinprick
 * MotorB - Horizontal Movement Stimulator ca. 0.3cm/Turn
 * D2 - Enable MotorA
 * D3 - Enable MotorB
 * D12 - MotorA-Position
 *
 * Inputs from Matlab:
 * A0 - Trigger Ch6
 * A1 - Trigger Ch5
 * A2 - Trigger Ch4
 * A3 - Trigger Ch3
 *
 * Outputs to Matlab
 * A4 - Trigger Ch2 - Trigger Blunt-Trial
 *
 * Trigger #32(6)(A0) - Reserved for Pinprick-Commands, no Command without it
 * Trigger #36(6+3)(A0+A3) - Start Trial
 * Trigger #40(6+4)(A0+A2) - MotorA goes to Zero-Position
 * Trigger #48(6+5) - MotorB Reset Position
 */


#include <Stepper.h>

const int stepsPerRevolution = 200; // How many steps are one round (Motor Specification)
const int minwalk = 100; // How many steps at least to be taken by MotorB

int Position = 0; // MotorB Position
int walk = 0;
const int maxPosition = 9 * stepsPerRevolution; // ca. 0.3cm/Turn, depends on hand length

//Initialize Motors with Stepper.h-Library
Stepper MotorA(stepsPerRevolution, 4, 5, 6, 7);
Stepper MotorB(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // put your setup code here, to run once:

  pinMode(2, OUTPUT); // enable MotorA
  pinMode(3, OUTPUT); // enable MotorB
  pinMode(A0, INPUT); // Pinprick-Command
  pinMode(A1, INPUT); // Start Trial
  pinMode(A2, INPUT); // MotorA Zero-Position
  pinMode(A3, INPUT); // MotorB Reset Position

  /*// Relax tension on silicon tubes
  digitalWrite(2, HIGH);
  MotorA.setSpeed(100);
  MotorA.step(stepsPerRevolution*5);
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  MotorB.setSpeed(100);
  MotorB.step(stepsPerRevolution*5);
  digitalWrite(3, LOW);*/

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

if(digitalRead(A0)==1 && digitalRead(A3)==1){
  digitalWrite(2, HIGH); // enable MotorA
  MotorA.setSpeed(100);
  while(digitalRead(12)==1) MotorA.step(1); // compensate resistance, start at zero
  delay(500);
  MotorA.step(stepsPerRevolution/2); // negative Steps, because the rod lays on the tubes left side
  digitalWrite(A4, HIGH);
  delay(10);
  digitalWrite(A4, LOW);
  MotorA.step(stepsPerRevolution/2);
  while(digitalRead(12)==1) MotorA.step(1); // compensate turn resistance, to do exactly one turn
  digitalWrite(2, LOW); // disable MotorA

  MotorB.setSpeed(200); // faster results in less torque
  walk = random(0-Position, maxPosition-Position); // Stimulator stays in defined Area(0-maxPosition), if MotorB Positon was reseted (e.g. to Knuckle-Line)
  while(abs(walk)<minwalk){ // walk has to exceed minwalk
    walk = random(0-Position, maxPosition-Position);
  }
  Position += walk;
  digitalWrite(3, HIGH); // enable MotorB
  MotorB.step(walk);
  digitalWrite(3, LOW); // disable MotorB

}

if(digitalRead(A0)==1 && digitalRead(A2)==1){ // get MotorA in Zero Position, Trigger #40
  while(digitalRead(12)==1){
    digitalWrite(2, HIGH); // enable MotorA
    MotorA.setSpeed(100);
    MotorA.step(1);
    digitalWrite(2, LOW); // disable MotorA
  }
  if(digitalRead(A0)==1 && digitalRead(A2)==1){ // get MotorA in Zero Position, Trigger #40
    digitalWrite(2, LOW); // disable MotorA
  }
}

if(digitalRead(A0)==1 && digitalRead(A1)==1){
  Position = 0; // MotorB Reset Position, Trigger #48
  MotorB.setSpeed(200);
  digitalWrite(3, HIGH); // enable MotorB
  MotorB.step(stepsPerRevolution);
  MotorB.step(stepsPerRevolution);
  digitalWrite(3, LOW); // disable MotorB
}

}
