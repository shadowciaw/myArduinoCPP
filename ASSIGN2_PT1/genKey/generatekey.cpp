#include <Arduino.h>
const int idPin = 1;

int generate_key(){
    // generates 16 bit private key from idPin

    // output variable
    int key = 0;

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
    Serial.printl(public_key);

    // ---

    while (Serial3.available() == 0){}
    // stop here until a key is printed on other serial

    //int B = Serial3.read();
    uint16_t B = readUnsigned16();

    int k = pow(B, a);
    //shared secret key: k = B^a

    return k;
}

int main() {
    setup();
    return 0;
}