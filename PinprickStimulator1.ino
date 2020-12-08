/*
 * Pinprick-Motor Control
 * MotorA - Vertical Movement Pinprick
 * MotorB - Horizontal Movement Stimulator
 */


#include <Stepper.h>

const int stepsPerRevolution = 200;

Stepper MotorA(stepsPerRevolution, 4, 5, 6, 7);
Stepper MotorB(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // put your setup code here, to run once:

  pinMode(A2, INPUT); // MotorA Positioning Input
  pinMode(A3, OUTPUT); // enable MotorA
  pinMode(A4, OUTPUT); // enable MotorB

  // get MotorA in Zero Position
  while(digitalRead(A2)==1){
    digitalWrite(A3, HIGH); // enable MotorA
    MotorA.setSpeed(150);
    MotorA.step(1);
  }
  digitalWrite(A3, LOW); // disable MotorA

  Serial.begin(9600);
}


void loop() {
  // put your main code here, to run repeatedly:

if(digitalRead(3)){
  if(digitalRead(2)){
    digitalWrite(A3, HIGH); // enable MotorA
    digitalWrite(A4, LOW); // disable MotorB
    // read the sensor value:
    int sensorReading = analogRead(A0);
    // map it to a range from 0 to 100:
    int motorSpeed = map(sensorReading, 0, 1023, 0, 100);
    // set the motor speed:
    if (motorSpeed > 0) {
      MotorA.setSpeed(motorSpeed);
      MotorA.step(stepsPerRevolution);
    }
  }else{
    digitalWrite(A3, LOW); // disable MotorA
    digitalWrite(A4, HIGH); // enable MotorB
    // read the sensor value:
    int sensorReading = analogRead(A0);
    // map it to a range from 0 to 100:
    int motorSpeed = map(sensorReading, 0, 1023, 0, 100);
    // set the motor speed:
    if (motorSpeed > 0) {
      MotorB.setSpeed(motorSpeed);
      MotorB.step(stepsPerRevolution);
    }
  }

}else{
  digitalWrite(A3, LOW);
  digitalWrite(A4, LOW);
}
}
