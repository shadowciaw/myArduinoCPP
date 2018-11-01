/*
	Basic template of an Arduino program to be compiled with the Makefile.
	All your code should have a main() function.

	Do not forget to copy the Makefile to your working directory,
	e.g. cp ~/arduino-ua/Makefile_Example/Makefile .
	if you are currently in the directory with your .cpp code.

    This example shows you how to use the 'serial-mon' program so your
    PC can communicate with an Arduino over serial port #0
    After you have 'compiled' using 'make' and uploaded the code
    to your arduino with 'make upload' you can start the 'serial-mon' program.
    
    To see serial-mon help use 'Ctrl+a z' 
    (i.e. press & hold the control key then
    press 'a' then 'z'  this brings up a menu of options.

    'Ctrl+a x' exits serial-mon

    In summary, the command line steps are (the '$' indicates command line):
    1)   $ make  2) $ make upload  3) $ serial-mon 
    to quit serial-mon 4) $ 'Ctrl+a x' 
*/

// otherwise various functions and identifiers are not defined when compiling
#include <Arduino.h>

// the variables for various pins should be defined as constant here
const int ledPin = 13;

// if you *need* other global variables, declare them here


// should have this function to initialize things
void setup() {
	// Always required, otherwise your program likely will not work.
	// Initializes a bunch of low-level functionality of the Arduino.
	init();

	// Must initialize serial port communications before using Serial.
	Serial.begin(9600);  // speed of communicating bits/second over serial line
	// forgetting this line = will not communicate

	// initialize pins
	pinMode(ledPin, OUTPUT);

	// initialize other devices (e.g. the display or SD card reader)
}


// REQUIRED!
int main() {
	setup(); // our first act should be to initialize everything

	/*
	  As this variable is not required across functions, it should be local.
	  NOTE: this does not mean any variable required across functions
		should be global. Perhaps the data can be passed as a function parameter
		or returned from a function. Usually it should be!
	*/
	int blinkPeriod = 1000;

	/*
	  It is ok if you have a function called loop() and repeatedly call
	  it within a loop like this, but this template shows you do not need
	  such a function
	*/
	while (true) {
		// now do whatever you need to do in your program!

		Serial.println("LED ON");       // prints to serial-mon
		digitalWrite(ledPin, HIGH);
		delay(blinkPeriod);

		Serial.println("LED OFF");      // prints to serial-mon
		digitalWrite(ledPin, LOW);
		delay(blinkPeriod);
	}

	/*
		We probably won't get here in most Arduino programs, but let's get
	  into the habit of return 0 anyway. It is more important with terminal-based
		programs that we will develop later in the course.
	*/
	return 0;
}
