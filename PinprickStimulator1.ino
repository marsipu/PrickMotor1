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
 * A1 - Trigger Ch1 Output
 * A2 - Trigger Ch2 Input
 * A3 - Trigger Ch3 Input
 * A4 - Trigger Ch4 Output
 * A5 - Trigger Ch5 Output
 *
 * Trigger #32(6)(A0) - Reserved for Pinprick-Commands, no Command without it
 * Trigger #36(6+1)(A0+A1) - Start Trial
 * Trigger #40(6+4)(A0+A4) - MotorA goes to Zero-Position
 * Trigger #48(6+5)(A0+A5) - MotorB Reset Position
 *
 */


#include <Stepper.h>

const int stepsPerRevolution = 200; // How many steps are one round (Motor Specification)
const int minwalk = 100; // How many steps at least to be taken by MotorB
const int maxPosition = 9 * stepsPerRevolution; // ca. 0.3cm/Turn, depends on hand length

int Position = 0; // MotorB Position
int walk = 0;
int MotorAon = 0;
int changevelo = 0;


//Initialize Motors with Stepper.h-Library
Stepper MotorA(stepsPerRevolution, 4, 5, 6, 7);
Stepper MotorB(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // put your setup code here, to run once:

  pinMode(2, OUTPUT); // enable MotorA
  pinMode(3, OUTPUT); // enable MotorB
  pinMode(A0, INPUT); // Pinprick-Command
  pinMode(A1, INPUT); // Start Trial
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

// Program 1: 1s Trials, Duration:
if(digitalRead(A0)==1 && digitalRead(A1)==1 && digitalRead(A4)!=1 && digitalRead(A5)!=1){ // #33

  delay(random(1000));

  if(changevelo==0){
    MotorA.setSpeed(137);
    digitalWrite(A2, HIGH);
    delay(10);
    digitalWrite(A2, LOW);
    changevelo=1;
  }else{
    MotorA.setSpeed(147);
    digitalWrite(A3, HIGH);
    delay(10);
    digitalWrite(A3, LOW);
    changevelo=0;
  }

  digitalWrite(2, HIGH); // enable MotorA
  while(digitalRead(12)==1) MotorA.step(1); // compensate resistance, start at zero
  MotorA.step(stepsPerRevolution); // negative Steps, because the rod lays on the tubes left side
  while(digitalRead(12)==1) MotorA.step(1); // compensate turn resistance, to do exactly one turn
  digitalWrite(2, LOW); // disable MotorA

  delay(3000);

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

// Program 2: Offset Response
if(digitalRead(A0)==1 && digitalRead(A1)==1 && digitalRead(A4)==1 && digitalRead(A5)!=1){ //#41

    delay(random(1000));

    if(changevelo==0){
      MotorA.setSpeed(137);
      digitalWrite(A2, HIGH);
      delay(10);
      digitalWrite(A2, LOW);
      changevelo=1;
    }else{
      MotorA.setSpeed(147);
      digitalWrite(A3, HIGH);
      delay(10);
      digitalWrite(A3, LOW);
      changevelo=0;
    }

    digitalWrite(2, HIGH); // enable MotorA

    while(digitalRead(12)==1) MotorA.step(1); // compensate resistance, start at zero
    MotorA.step(stepsPerRevolution/2);
    digitalWrite(2, LOW); // disable MotorA
    delay(2000 + random(1000));
    digitalWrite(A2, HIGH); //Trigger Offset
    digitalWrite(A3, HIGH);
    delay(10);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);
    digitalWrite(2, HIGH); // enable MotorA
    MotorA.step(stepsPerRevolution/2); //Offset Response
    while(digitalRead(12)==1) MotorA.step(1); // compensate turn resistance, to do exactly one turn
    digitalWrite(2, LOW); // disable MotorA

    delay(3000);

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


if(digitalRead(A0)==1 && digitalRead(A4)==1 && digitalRead(A1)!=1 && digitalRead(A5)!=1 && MotorAon==0){ // get MotorA in Zero Position, Trigger #40
  while(digitalRead(12)==1){
    digitalWrite(2, HIGH); // enable MotorA
    MotorA.setSpeed(100);
    MotorA.step(1);
  }
  MotorAon = 1;
}


if(digitalRead(A0)==1 && digitalRead(A4)==1 && digitalRead(A1)!=1 && digitalRead(A5)!=1 && MotorAon==1){ // get MotorA in Zero Position, Trigger #40
  MotorAon = 0;
  digitalWrite(2, HIGH); // enable MotorA
  MotorA.step(stepsPerRevolution);
  digitalWrite(2, LOW); // disable MotorA
}


if(digitalRead(A0)==1 && digitalRead(A5)==1 && digitalRead(A1)!=1 && digitalRead(A4)!=1){ // #48
  Position = 0; // MotorB Reset Position, Trigger #48
  MotorB.setSpeed(200);
  digitalWrite(3, HIGH); // enable MotorB
  MotorB.step(stepsPerRevolution);
  MotorB.step(-stepsPerRevolution);
  digitalWrite(3, LOW); // disable MotorB
}

if(digitalRead(A0)==1 && digitalRead(A5)==1 && digitalRead(A1)==1 && digitalRead(A4)!=1){ // #49
  Position = 0; // MotorB Reset Position, Trigger #48
  MotorA.setSpeed(137);
  digitalWrite(2, HIGH); // enable MotorA
  MotorA.step(stepsPerRevolution);
  digitalWrite(2, LOW); // disable MotorA
  delay(1000);
  MotorA.setSpeed(147);
  digitalWrite(2, HIGH); // enable MotorA
  MotorA.step(stepsPerRevolution);
  digitalWrite(2, LOW); // disable MotorA
  delay(1000);
  MotorB.setSpeed(200);
  digitalWrite(3, HIGH); // enable MotorB
  MotorB.step(stepsPerRevolution);
  digitalWrite(3, LOW); // disable MotorB
}

}
