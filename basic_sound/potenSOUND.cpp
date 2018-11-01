#include <Arduino.h>

int speakerPin = 7;
int controlPin = 6;



void setup() {
    pinMode(speakerPin, OUTPUT);
}

// play a tone with period given in microseconds for a duration 
// given in milliseconds

void playTone(int period, int duration) {
    // elapsed time in microseconds, the long is needed
    // to get a big enough range

    long elapsedTime = 0;  // int with BIG bits

    // note we're making a slight error here if period isnt given
    int halfPeriod = period / 2;

    while (elapsedTime < duration * 1000L) {
        // generate a square wave of the given period
        // Half of the period is on, other half off

        // 1000L means 1000 is a LONG n

        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(halfPeriod);

        digitalWrite(speakerPin, LOW);
        delayMicroseconds(halfPeriod);

        // count the time we just consumed for 1 cycle of tone
        elapsedTime = elapsedTime + period;
    }
}

void loop() {
    int period;

    period = analogRead(controlPin);
    playTone(period, 500);

    int mappedVal = map(period, 0, 1000L, 100L, 10000L)
}