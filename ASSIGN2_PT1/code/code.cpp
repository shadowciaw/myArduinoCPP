/*
    Name: Cynthia Li, Cyrus Diego
    ID: 1529078
    CMPUT 274 FALL 2018

    Assignment 2 part 1: proof of concept
*/

#include <Arduino.h>

// to be used to genereate random private key
// not connected cause numbers to fluctuate
const int idPin = 1;

uint16_t skey;

void readString(char str[], int len)
{
    // function header //

    int index = 0;

    // using while instead of for bc wanna know where to put null terminal when while loop ends
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
    // index is at end of str, now add end to it
    // is null terminator
    str[index] = '\0';
}

int readUnsigned16()
{
    // FUNCTION HEADER //

    // creates array of length 16 named str
    char str[16];

    // read from serial terminal and produce

    // wait nothing was returned in these two functions, str has been made and appended but we do nothing with it???
    readString(str, 16);
    return atol(str);
    // why can a number that fits unsigned long but not a signed long be returned
    // when atol returns only signed long

    // is it bc the function declares it as unsigned long??
    // also, if we're working in 16 bits do we need atol
    // which returns a long 32bit integer???
}

int generate_key()
{
    // generates 16 bit private key from idPin

    // output variable
    int key = 0;

    for (int i = 0; i < 16; i++)
    {
        int bit = analogRead(idPin) & 1;
        // obtains least significant bit of analogRead
        key |= (bit << i);
        // shorthand for key = key | (bit << i );
        // << is operator for binary shifting. 1 << 2 shifts 1 from 2^0 to 2^2

        delay(50); // 50 ms
    }

    return key;
}

uint32_t powMod(int g, uint16_t private_key, int p)
{
    // FUNCTION HEADER //
    
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
    // function header //

    init();

    // set idPin to INPUT and turn on the internal pullup resistor
    pinMode(idPin, INPUT);
    digitalWrite(idPin, HIGH);

    Serial.begin(9600);
    Serial3.begin(9600);

    Serial.println("Welcome to Arduino chat");

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
    while (Serial.available() == 0)
    {
    }
    // stop here until a key is printed on other serial

    int B = readUnsigned16();
    // B is public key of other user
    Serial.println(B);

    skey = powMod(B, private_key, p);
    //shared secret key: k = B^a
    Serial.print("Your shared key is: ");
    Serial.println(skey);
    Serial.println();
    Serial.println();
}

uint8_t encr(uint8_t byte) {
    // encrypts/decrypts byte of information

    uint8_t encrypted = byte ^ ((uint8_t) skey);
    // encrypts byte using xor encryption with computed shared key

    return (encrypted);
}

void send() {
    // FUNCTION HEADER //

    uint8_t c = Serial.read();
    // reads from keyboard, stores character as binary

    Serial.write(c);
    // prints to own serial what was typed

    if ((int) c == 13) {
        // if carriage return was entered
        Serial3.write(encr(c));
        Serial3.write(encr('\n'));
        Serial.println();
        // sends \r and \n to other serial
        // prints newline to own serial
    }
    if ((int) c != 13) {
        Serial3.write(encr(c));
        // otherwise, send character to other serial monitor
    }

}

void receive() {
    // FUNCTION HEADER //
    
    uint8_t c = Serial3.read();
    // reads from serial, is in binary

    uint8_t decr = encr(c);
    // decrypts read data

    Serial.write(decr);
    // write ascii character to serial monitor
}

int main()
{
    // FUNCTION HEADER //

    setup();
    // shared secret key k returned from setup

while (true) {
    if (Serial.available()) {
        // if something is being typed by user
        send();
    }
    if (Serial3.available()){
        // if something is being typed by other user
        receive();
    }
}

    return 0;
}