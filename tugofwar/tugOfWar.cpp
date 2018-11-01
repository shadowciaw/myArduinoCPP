/*
    Arduino Intro Labs - Lab 2 Tug of War Oct. 23, 2018 R.Kube
    This program implements the 'Tug of war game:
    
    Two players, Player 1 and Player 2, are at a board
    consisting of 2 push buttons and 5 LEDS

    There are 5 LEDs
        (Player 1) RED RED GRN RED RED (Player 2)
    and 2 buttons
        B1 B2

    The objective is to push your button faster than your opponent
    so that when time runs out, you have the rope on your side.

    Whoever has pushed their button the most times will be
    winning, and their red LED will turn on.  If the two buttons
    are within Threshold of each other, then the players are
    currently tied, and the green LED will be on.

    Note: the LED and Button numbers match the 'Counting Lights' weekly
    exercise, and not the Lab 2 pins (which were 8, 9, 10 for LEDs
    pin 9 was green, now pin 11 is green, and P1ButtonPin is now pin 6)	

*/

// otherwise various functions and identifiers are not defined when compiling
#include <Arduino.h>

// the variables for various pins should be defined as constant here
const int P1LEDHighPin = 13;        // Player 1 red LED high
const int P1LEDLowPin = 12;         // Player 1 red LED low
const int LEDEvenPin = 11;          // Players even green LED
const int P2LEDLowPin = 10;         // Player 2 red LED low
const int P2LEDHighPin = 9;         // Player 2 red LED high

const int P1ButtonPin = 6;          // Player 1 push button
const int P2ButtonPin = 7;          // Player 2 push button

// if you *need* other global variables, declare them here

void setup() {
	init();

    // config LED pins to be a digital OUTPUT
    pinMode(P1LEDHighPin, OUTPUT);
    pinMode(P1LEDLowPin, OUTPUT);
    pinMode(LEDEvenPin, OUTPUT);    
    pinMode(P2LEDLowPin, OUTPUT);
    pinMode(P2LEDHighPin, OUTPUT);
    
    // configue the button pins to be a digital INPUT
    // turn on the internal pull-up resistors
    pinMode(P1ButtonPin, INPUT);
    digitalWrite(P1ButtonPin, HIGH);
    pinMode(P2ButtonPin, INPUT);
    digitalWrite(P2ButtonPin, HIGH);

}


int main() {
	setup(); // our first act should be to initialize everything

/*  PushCount is incremented when Player1 pushes their
    button, and decrements when Player2 pushes their button.
    So, if it is > 0 Player1 has made more button pushes
    and if it is < 0 Player2 has made more button pushes.
*/
	int PushCount = 0;

/*  PushCount has to be 'Threshold' amount from zero before a
    player is considered to be winning.  'HighThreshold' is winning big!
*/
	int Threshold = 5;
    int HighThreshold = 10;


	while (true) {
		// increment or decrement PushCount
        if ( digitalRead(P1ButtonPin) == LOW ) {
            PushCount++;
        }
        if ( digitalRead(P2ButtonPin) == LOW ) {
            PushCount--;
        }

        // Determine who is winning
        if ( PushCount > Threshold ) {
            digitalWrite(P1LEDLowPin, HIGH);        // Player 1 winning
            digitalWrite(LEDEvenPin, LOW);
            digitalWrite(P2LEDLowPin, LOW);
            
            if ( PushCount > HighThreshold ) {      // Player 1 winning big
                digitalWrite(P1LEDHighPin, HIGH);
            } else {
                digitalWrite(P1LEDHighPin, LOW);
            }
        } else if ( PushCount < (0-Threshold) ) {
            digitalWrite(P1LEDLowPin, LOW);
            digitalWrite(LEDEvenPin, LOW);
            digitalWrite(P2LEDLowPin, HIGH);        // Player 2 winning

            if ( PushCount < (0-HighThreshold) ) {  // Player 2 winning big
                digitalWrite(P2LEDHighPin, HIGH);
            } else {
                digitalWrite(P2LEDHighPin, LOW);
            }          
        } else {
            digitalWrite(P1LEDHighPin, LOW);
            digitalWrite(P1LEDLowPin, LOW);
            digitalWrite(LEDEvenPin, HIGH);         // Game tied
            digitalWrite(P2LEDLowPin, LOW);
            digitalWrite(P2LEDLowPin, LOW);
        }

        // wait 50 ms for any button bounce to die out
        delay(50);

    }  // end while true  


	return 0;

}  // end main

