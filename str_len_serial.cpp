/*
	A function that prints a string literal to the serial port followed by
	the length of the string literal.

    Make sure you understand each example (hint: useful for assign #2)
*/

#include <Arduino.h>

void setup() {
	init();
	Serial.begin(9600);
}

/*
	Print str to the serial port and the number of bytes printed on one line
	ending with a newline. Then print a blank line after.

	Example: calling nameAndLength("arduino") should print the following
	arduino 7
*/
void nameAndLength(const char* str) {
	int numBytes = Serial.print(str);
	Serial.print(" ");
	Serial.println(numBytes);

	Serial.println(); // print a blank line after
}

int main() {
	setup();

	nameAndLength("arduino");
	nameAndLength("four");
	nameAndLength("spaces count");
	nameAndLength(" ");

	// print a blank line between the normal examples and the tricky ones
	Serial.println();

	// some tricky examples to uncomment and play with, 
    // can you figure out why they do this?
    // hint: https://en.cppreference.com/w/cpp/language/escape
	nameAndLength("tricky\nexample\n");
	nameAndLength("this is also tricky\r");
	nameAndLength("this\r\nis\r\njust\r\na\r\nbit\r\ntricky");
	nameAndLength("twilight: new moon\b");

	// Serial.print will print one character at a time until it hits
	// the 0-byte, which is automatically added to the end of every string
	// literal but we can add it with \0
	nameAndLength("very tricky\0 do you know what happened?");

	Serial.flush();

	return 0;
}
