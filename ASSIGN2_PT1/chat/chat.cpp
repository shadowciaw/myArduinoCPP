/*
	Talking arduinos example.
    This example shows you how to print to Serial (a.k.a Serial0)
    and Serial3.

    Learn about Arduino Serial functions:
    https://www.arduino.cc/en/Serial/Read

*/

#include <Arduino.h>

const int idPin = 8;

void setup()
{
	init();
	// set idPin to INPUT and turn on the internal pullup resistor
	pinMode(idPin, INPUT);
	digitalWrite(idPin, HIGH);

	Serial.begin(9600);
	Serial3.begin(9600);
}

void sender()
{
	while (true) {
        // wait until some data has arrived
		while (Serial.available() == 0) { } 
        // availble: "if theres character avaible for me to read on serial port 0, read. if nothing, spin and wait for character"
		char byteRead = Serial.read();

        if (Serial.available()) {
            char reading = Serial.read();
            Serial.print(reading);
        }

		//TODO: after running the program once,
        // uncomment the HEX, DEC, BIN print format and
		// check the ascii table at www.asciitable.com
		// for each character you press on the keyboard.
        // Q1: Is #1 below different from #3?
        // Q2: #1 is an example of 'type casting'. What does that mean?
        //      hint: search 'c++ type casting'

        // check for CR i.e. '\r'
		if ((int) byteRead == 13) {
			Serial.println(); // char return is acsii 13
		}
		else {
			  Serial.println(byteRead);

		}
	}

	Serial.flush();
}

void receiver()
{
	Serial.println("I am Bob the receiver");

	// wait until some data has arrived
	while (true)
	{
		// wait until the sender has sent some data
		while (Serial3.available() == 0)
		{
		}
		char byteRead = Serial3.read();

		// the sender sends \r\n with println()
		// so we don't need to add our own \n
		Serial.print(byteRead);
	}
}

int main()
{
	setup();

		while (true)
		{
			//if (Serial.available() == 0) {}
			// char byte3Read = Serial.read();
			// Serial3.print(byte3Read);
			
			if (Serial.available()) {
				char reading = Serial.read();
			}

			if (Serial3.available() == 0)
			{
			}
			char byteRead = Serial3.read();
			Serial.print(byteRead);
		}


// TODO:
// comment out the if/else part above
// create your own unecrypted chat!
// have an infinite loop (while (true)) that, in each iteration,
// checks if something is available from Serial and print it
// to Serial3, and also if something is available from Serial3
// and print it to Serial

Serial3.flush();
Serial.flush();

return 0;
}
