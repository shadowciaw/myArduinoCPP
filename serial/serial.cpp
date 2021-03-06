/*  This example uses 'serial-mon' to display print statements
    to your PC's screen.

    Compile & upload to your arduino using make & make upload
    See if you can answer the four questions Q1 - Q4
*/

#include <Arduino.h>

void setup() {
	init();
	Serial.begin(9600);
}

int main() {
	setup();

	Serial.println("hello world!");
	Serial.println("second message");

	// an example of using a string literal variable
	const char* message = "string literal";
	Serial.println(message);

	// single quotes for a character literal
	// char character = "a"; ERROR
	// char character ='asdfafasx'; Q1: does this work? no...
	char character = 'z';
	Serial.println(character);

	char character = 'abcde';
	Serial.println(character);

    // escape sequences
    // see https://en.cppreference.com/w/cpp/language/escape
	Serial.print("goto\r\nnewline\r\n");
	Serial.print("\"double quotes\"");

	// just start a new line
	Serial.println();

	// carriage return + linefeed examples
	Serial.println("carriage\rreturn");  // Q2: why does this print 'returnge'?
	Serial.println("line\nfeed");

	// audible bell?
	Serial.print("\a");

	int numBytes = Serial.print("abcde");
	Serial.println(numBytes);

    // Q3: why does this print '4' after "hi"?
    // hint: https://www.arduino.cc/en/Serial/Println
	numBytes = Serial.println("hi");
	Serial.println(numBytes);

	// wait until the Serial port has printed all
	// "buffered" messages
    // while (true) {};
 
    // Q4: w/o the while loop or serial.flush why doesn't
    // the arduino print out all the print statements?
	Serial.flush();
    
	return 0;
}
