#include <elapsedMillis.h>

const int dirPin = 3;
const int stepPin = 4;
const int stepsPerRevolution = 200;
elapsedMillis stepTimer;
int stepCount = 0;
const int stepInterval = 5; // 5000 microseconds

void motorSetup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void motorLoop() {
  if (stepCount < stepsPerRevolution && stepTimer >= stepInterval) {
    stepTimer = 0; // Reset the timer
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1); // Short pulse
    digitalWrite(stepPin, LOW);
    stepCount++;
  } else if (stepCount >= stepsPerRevolution) {
    stepCount = 0; // Reset for the next revolution
  }
}
