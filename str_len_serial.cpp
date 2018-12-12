
/*
	A function that prints a string literal to the serial port followed by
	the length of the string literal.

    Make sure you understand each example (hint: useful for assign #2)



#include <Arduino.h>

void setup()
{
	init();
	Serial.begin(9600);
}


	Print str to the serial port and the number of bytes printed on one line
	ending with a newline. Then print a blank line after.

	Example: calling nameAndLength("arduino") should print the following
	arduino 7

void nameAndLength(const char *str)
{
	int numBytes = Serial.print(str);
	Serial.print(" ");
	Serial.println(numBytes);

	Serial.println(); // print a blank line after
}


int main()
{
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

	unsigned char a;
	Serial.println(sizeof(a));
	// 1 byte

	char b[] = "2016";
	Serial.println(sizeof(b));
	// 4 bytes
	// nope its 5 bytes haha

	uint16_t c[] = {1, 2, 1, 6};
	Serial.println(sizeof(c));
	// 8 bytes

	int d[4] = {'2', '0', '1', '6'};
	Serial.println(sizeof(d));
	// 8 bytes

	int f[2][3];
	Serial.println(sizeof(f));
	// 12 bytes

	Serial.flush();

	return 0;
}
*/

#include <Arduino.h>

/*
    Read Joystick oriented left of the breadboard with
	breadboard on left side

	reads two analog pins that are supposed to be 
	connected to a joystick made of two potentiometers

	send 2 bytes to serial mon: x and y
	sends 00 if in default position
	x is positive if move to right
	y is positive if moved up
	
*/

//int ledPin = 13;
const int Xpin = A0;
const int Ypin = A1;
int valx = 0;
int valy = 0;

void setup()
{
	init();
	//pinMode(ledPin, OUTPUT);
	// initialize digital pin 0 to 7 as outputs
	Serial.begin(9600);
}


int treatValue(int data)
{
	// transform sensor's measurement into value between 0 and 4
	// first 
	return ((-1)*(data * 9 / 1024 - 4));
	
}

int main()
{

	setup();

	while (true)
	{
		// 

		valx = analogRead(Xpin);
		//delay(100);
		valy = analogRead(Ypin);

		Serial.print(treatValue(valx));
		Serial.println((-1)*treatValue(valy));

		delay(10);

	return 0;
}