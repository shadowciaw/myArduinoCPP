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
    uint8_t encrypted = byte ^ ((uint8_t) skey);

    return (encrypted);
}

void send() {
    uint8_t c = Serial.read();
    // reads from keyboard

    Serial.write(c);
    // prints to own serial what was typed

    // Serial.write(encr(encr(c)); works properly

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
    }

}

void receive() {
    uint8_t c = Serial3.read();
    // reads from serial, is in bytes

    uint8_t decr = encr(c);
    Serial.write(decr);
}

int main()
{
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

/*
    while (true)
    {
        // use exlusive-or operation (^) for encryption/decryption

        // given a message character m, compute character e by exclusive or
        // with first 8 bits:
        // klow = k mod 256
        // of key k: e = m (exclusive or) klow

        // since exclusive or is its own inverse, original message character m
        // can be obtained from encrypted e with klow: m = e (exclusive or) klow

        if (Serial.available() > 0)
        // if user has typed something in serial, send it to other user
        {
            uint8_t byte = Serial.read();
            // is byte. can be in ascii if print and char if write

            Serial.write(byte);
            // displays what the user typed

            if ((int)byte == 13)
            {
                Serial3.print('\r' ^ ((uint8_t)skey));
                Serial3.print('\n' ^ ((uint8_t)skey));
                Serial.println();
            }
            else
            {
                uint8_t encryptedByte = byte ^ ((uint8_t)skey);
                Serial3.print(encryptedByte);
            }
        }

        if (Serial3.available() > 0)
        // if other user has typed something in serial, decrypt it and print in serial
        {
            while (true)
            {
                uint8_t encryptedByte = Serial.read();
                uint8_t decryptedByte = encryptedByte ^ ((uint8_t)skey);

                Serial.write(decryptedByte);
                // serial.write prints character represented by ascii
                // serial.print prints ascii number
                // serial.read returns ascii char
                if ((int)encryptedByte == 13)
                {
                    break;
                }
            }
        }
    }

    */

    return 0;
}