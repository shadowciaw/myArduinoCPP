#include <Arduino.h>

// the LED is attached to this digital output pin
int ledPin = 13;

// the pushbutton is attached to this digital input pin
int buttonPin = 8;

// the value read by the pin.
int buttonValue;

// V2: the value to be sent to the LED
int lightState;

void setup() {
    //set ledPin to OUTPUT
    pinMode(ledPin, OUTPUT);

    // set buttonPin to INPUT and turn on internal pull up resistor
    pinMode(buttonPin, INPUT);
    digitalWrite(buttonPin, HIGH);
}

void loop() {
    // read the pushbutton state and set the light to be the same value
    buttonValue = digitalRead(buttonPin);

    // V2: if you want the light on when the pushbutton is pushed
    if ( buttonValue == LOW ) {
        lightState = HIGH;
        buttonValue = LOW;
    }
    else {
        lightState = LOW;
        buttonValue = LOW;
    }

    // V2: and set the light to be the same value
    digitalWrite(ledPin, lightState);
}