/*
+---------------------------------------------------+
| NAME: Cynthia Li, Cyrus Diego                     |
| ID: 1529078, 1528911                              |
| CMPUT 274 FALL 2018                               |
|                                                   |
| Assignment 2 Part 1: Proof of Concept             |
|                                                   |
| This program allows two users to communicate      |
| through the serial port of two Arduinos.          |
+---------------------------------------------------+
*/

#include <Arduino.h>

const int idPin = 1;  // Used for generation of random private key

uint16_t skey;  // Used to store shared secret key


void readString(char str[], int len)
{
    /*
    readString() takes in a char array and the length of the array
    Used to read input from Serial when inputting the individual private
    keys
    */

    int index = 0;

    // while looping through array, append to it with the user input
    while (index < len - 1)
    {
        // as long as we're not done reading all of char str array
        if (Serial.available() > 0)
        {
            // if theres something to be read on Serial
            char byteRead = Serial.read();

            // checks if user pressed enter, if so, break
            if ((int)byteRead == 13)
            {
                break;
            }
            else
            {
                // append byte to str array
                str[index] = byteRead;
                ++index;
            }
        }
    }

    // after while loop is done, index will be at end of array,
    // appends the null terminator at the end
    str[index] = '\0';
}


int readUnsigned16()
{
    /*
    readUnsigned16() used with readString() to accept input from
    user for typing in other arduino's private key.
    */
    
    char str[16];   // creates array of length 16 named str
    readString(str, 16);    // calls readString() function

    return atol(str); // returns the integer type of stringed together array
}


int generate_key()
{
    /*
    generate_key() generates a 16 bit private key from the idPin
    uses the fluctuation of number read from pin to generate a random number
    */

    int key = 0;    // output variable

    for (int i = 0; i < 16; i++)    // 16 iteratiosn -> 16 bits
    {
        int bit = analogRead(idPin) & 1;
        // obtains least significant bit of analogRead
        // & is binary and operator. if LSB of analogRead is 0, gets 0. otherwise 1

        key |= (bit << i);
        // shorthand for key = key | (bit << i ); | is or operator
        // << is operator for binary shifting. 1 << 2 shifts 1 from 2^0 to 2^2

        delay(50); // delay 50 ms
    }

    return key;
}


uint32_t powMod(int g, uint16_t private_key, int p)
{
    /*
    powMod() is used to generate the public key
    returns a 16 bit number regardless of it being defined as 32 bits
    as the input numbers are all 16 bits or less.
    */
    g = g % p;
    uint32_t public_key = 1 % p;
    for (uint32_t i = 0; i < private_key; i++)
    {
        public_key = (public_key * g) % p;
    }

    return public_key;
}


void setup()
{
    /*
    initial setup for program: generates keys for both Arduino's and
    displays text in both Serial-mon
    */

    init();

    // set idPin to INPUT and turn on the internal pullup resistor
    pinMode(idPin, INPUT);
    digitalWrite(idPin, HIGH);

    // begin serial mons
    Serial.begin(9600);
    Serial3.begin(9600);

    Serial.println("Welcome to Arduino chat");

    // generates the random private key and public key
    uint16_t private_key = generate_key();
    int p = 19211;
    int g = 6;
    uint32_t public_key = powMod(g, private_key, p);

    Serial.print("Your public key is: ");
    Serial.println(public_key);
    Serial.println();

    // --- done operating with own key, working on obtaining other key -- //
    Serial.println("Now enter the other Arduino's public key");
    Serial.print("It will not display until you press <enter>: ");

    // stop here until a key is printed on other serial
    while (Serial.available() == 0) {}

    int B = readUnsigned16();
    // B is public key of other user
    Serial.println(B);

    skey = powMod(B, private_key, p);
    //shared secret key: skey = B^a mod p

    Serial.print("Your shared key is: ");
    Serial.println(skey);
    Serial.println();
    Serial.println();
}


uint8_t encr(uint8_t byte)
{
    /*
    encr() is an encrypting/decrypting function that serves
    to encrypt outgoing messages and decrypts incoming messages
    */

    // encrypts using xor operator
    uint8_t encrypted = byte ^ ((uint8_t) skey);

    return (encrypted);
}


void send()
{
    /*
    send() is the function that sends messages to the other arduino,
    also prints to own user's screen as well as makes sure to encrypt the
    text
    */

    // reads from user keyboard input on local serial
    uint8_t text = Serial.read();

    // prints to own serial what was typed
    Serial.write(text);

    // if carriage return was entered: it will encrypt the text and
    // print/write a newline character to both Serials
    if ((int) text == 13)
    {
        Serial3.write(encr(text));
        Serial3.write(encr('\n'));
        Serial.println();
    }

    // if there's a character available, write encrypted char to Serial 3
    if ((int) text != 13)
    {
        Serial3.write(encr(text));
    }

}


void receive()
{
    /*
    receive() is the function that receives messages from the other arduino
    and decrypts as well.
    */
    
    // reads from serial3 (other arduino) which will be in bytes
    uint8_t text = Serial3.read();

    // decrypts the text and stores in decr
    uint8_t decr = encr(text);

    // writes the decrypted text into own serial and shows on screen
    Serial.write(decr);
}


int main()
{
    /*
    main() function only calls setup() and a while loop that constantly
    keeps send() and receive() running
    */

    // calls setup() which does all the background work for the encryption
    setup();

    while (true)
    {
        // if there's something available in serial/ something is being typed
        if (Serial.available())
        {
        send();
        }

        // if there is something being typed by other user
        if (Serial3.available())
        {
        receive();
        }
    }

    return 0;
}
