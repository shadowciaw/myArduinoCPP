#include <Arduino.h>
const int idPin = 1;

void readString(char str[], int len)
{
    int index = 0;

    // using while instead of for bc want to know where to put null terminal when while loop ends
    while (index < len - 1)
    {
        // as long as we're not done reading all of str
        if (Serial.available() > 0)
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
    // SHOULD THIS BE 32 INSTEAD OR????

    char str[16];
    readString(str, 16);
    return atol(str);
    // why can a number that fits unsigned long but not a signed long be returned
    // when atol returns only signed long

    // is it bc the function declares it as unsigned long??
    // also, if we're working in 16 bits do we need atol
    // which returns a long 32bit integer???
}

int generate_key(){
    // generates 16 bit private key from idPin

    // output variable
    uint16_t key = 0;

    for (int i = 0; i < 16; i++){
        bit[i] = analogRead(idPin, BIN) & 1;
        // obtains least significant bit of analogRead
        key |= (bit << i);
        // shorthand for key = key | (bit << i );
        // << is operator for binary shifting. 1 << 2 shifts 1 from 2^0 to 2^2

        delayTime(50); // 50 ms
    }
    // god knows wtf im doing 
    // is this an int of 1s and 0s or ???
    // how do i turn this into an int

    return key;
}

uint16_t setup()
{
    init();

    // set idPin to INPUT and turn on the internal pullup resistor
    pinMode(idPin, INPUT);
    digitalWrite(idPin, HIGH);

    Serial.begin(9600);
    Serial3.begin(9600);

    uint16_t private_key = generate_key();
    //rand() % pow(2.0, 16.0); // GENEREATE 16 BIT PRIVATE KEY
    int p = 19211;
    int g = 6;
    int public_key = pow(g, a) % p;

    // Serial.print("public key: ");
    Serial.println(public_key);

    // ---

    //while (Serial3.available() == 0){}
    // stop here until a key is printed on other serial

    //int B = Serial3.read();
    //uint16_t B = readUnsigned16();

    //int k = pow(B, a);
    //shared secret key: k = B^a

    return public_key;
}

int main() {
    uint16_t key = setup();
    return 0;
}