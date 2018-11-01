// the external LED is attached to this pin.
// onboard LED is attached to pin 13
#include <Arduino.h>

int ledPin = 13;

// slider of the potentiometer attached to this analog input
int analogInPin = 0;

// On and off time in milliseconds for the light
int delayTime = 300;

void setup() {
  // configure ledPin to be a digital output
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read the voltage on the analog input and convert
  // into an integer value in the range [0, 1023]
  delayTime = analogRead(analogInPin);

  digitalWrite(ledPin, HIGH);
  delay(delayTime);

  digitalWrite(ledPin, LOW);
  delay(delayTime);
}
