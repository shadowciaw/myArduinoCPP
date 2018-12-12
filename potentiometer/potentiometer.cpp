// the external LED is attached to this pin.
// onboard LED is attached to pin 13
#include <Arduino.h>

// slider of the potentiometer attached to this analog input
//int analogInPin = 0;

// On and off time in milliseconds for the light
//int delayTime = 300;

void setup() {
  // configure ledPin to be a digital output
  // pinMode(ledPin, OUTPUT);
  init();
  Serial.begin(9600);
}

int main() {

  setup();
  
  while (true) {
    Serial.print(2);
    Serial.print(2);
    delay(1000);
    //Serial.flush();
  }

  return 0;
}

// normal is 0 0

// up -4 0
// down 4 0

// left 