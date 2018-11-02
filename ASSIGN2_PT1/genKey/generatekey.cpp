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

void readString(char str[], int len)
{
    // function header //

    int index = 0;

    // using while instead of for bc wanna know where to put null terminal when while loop ends
    while (index < len - 1)
    {
        // as long as we're not done reading all of char str array
        if (Serial3.available() > 0)
        {
            // if theres something to be read on Serial
            char byteRead = Serial.read();

            // checks if user pressed enter, if so, break
            if (byteRead = '\r')
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

uint16_t readUnsigned16()
{
    // FUNCTION HEADER

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

uint16_t generate_key(){
    // generates 16 bit private key from idPin

    // output variable
    uint16_t key = 0;

    for (int i = 0; i < 16; i++){
        int bit = analogRead(idPin) & 1;
        // obtains least significant bit of analogRead
        key |= (bit << i);
        // shorthand for key = key | (bit << i );
        // << is operator for binary shifting. 1 << 2 shifts 1 from 2^0 to 2^2

        delayTime(50); // 50 ms
    }

    return key;
}

uint16_t setup()
{
    // function header //

    init();

    // set idPin to INPUT and turn on the internal pullup resistor
    pinMode(idPin, INPUT);
    digitalWrite(idPin, HIGH);

    Serial.begin(9600);
    Serial3.begin(9600);

    uint16_t private_key = generate_key();
    int p = 19211;
    int g = 6;
    int public_key = pow(g, private_key) % p;

    Serial.println(public_key);

    // --- done operating with own key, working on obtaining other key -- //

    while (Serial3.available() == 0){}
    // stop here until a key is printed on other serial

    uint16_t B = readUnsigned16();
    // B is public key of other user

    uint16_t k = pow(B, a);
    //shared secret key: k = B^a

    return k;
}


int main()
{
    uint16_t k = setup();
    // shared secret key k returned from setup

    Serial.flush();

    return 0;
}