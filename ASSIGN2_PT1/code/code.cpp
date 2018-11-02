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

    uint16_t private_key = generate_key();
    int p = 19211;
    int g = 6;
    uint32_t public_key = powMod(g, private_key, p);

    Serial.print("Your public key is: ");
    Serial.println(public_key);

    // --- done operating with own key, working on obtaining other key -- //
    Serial.print("Now enter the other user's public key: ");
    while (Serial.available() == 0)
    {
    }
    // stop here until a key is printed on other serial

    uint16_t B = readUnsigned16();
    // B is public key of other user

    skey = pow(B, private_key);
    //shared secret key: k = B^a
}

int encrypted(uint8_t byte) 
{
    // encrypts a byte

    int encryptedByte = byte ^ ((uint8_t)skey);

    return encryptedByte;
}

void readSend()
{
    // read a character (if available) from serial monitor
    // send it to the other machine ENCRYPTED With help of shared secret key k
    while (true)
    {
        // while serial is empty, wait.
        while (Serial.available() == 0)
        {
        }
        char byte = Serial.read();
        // ex. already a byte

        //Serial.write(byteread);
        // Serial.write writes binary byte to serial mon, which is interpreted as char

        // Serial3.write() // writes BINARY data to serial port
        // inside can be (val), (str), (buf, len)
        // value to send as single byte
        // str to send as series of bytes
        // array to send as series of bytes and length of buffer

        // do i really need this? its gonna print after every char right? how would i go about waiting for the entire thing then
        // if i was to do what was done in readString() id have to know the length of the message first dont i

        // why do they use uint32 and uint8 ??
        // why do we need to type cast it down to 8???
        // its 16 bits long how tf???

        if ((int)byte == 13)
        {
            // send \r (carriage return)
            Serial3.write(encrypted('\r'));

            // follow with line feed char \n
            Serial3.write(encrypted('\n'));
                // both are encrypted

                break;
        }
        Serial.print(byte);

        Serial3.write(encrypted(byte));

        Serial.flush();
        Serial3.flush();
    }
}

void receive()
{
    // receive an encrypted byte (if available) from other machine
    // decrypt it using shared secret key
    // send to serial monitor
    while (true)
    {
        while (Serial.available() == 0)
        {
        }
        char byteRead = Serial3.read();

        Serial.print(encrypted(byteRead));
        if ((int)byteRead == 13)
        {
            break;
        }
    }
    Serial.flush();
    Serial3.flush();
}

int main()
{
    setup();
    // shared secret key k returned from setup

    while (true)
    {
        // use exlusive-or operation (^) for encryption/decryption

        // given a message character m, compute character e by exclusive or
        // with first 8 bits:
        // klow = k mod 256
        // of key k: e = m (exclusive or) klow

        // since exclusive or is its own inverse, original message character m
        // can be obtained from encrypted e with klow: m = e (exclusive or) klow
        if (Serial.available()) {
            readSend();
        }

        if (Serial3.available()) {
            receive();
        }
    }

    Serial.flush();

    return 0;
}