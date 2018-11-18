/*
+---------------------------------------------------+
| NAME: Cynthia Li, Cyrus Diego                     |
| ID: 1529078, 1528911                              |
| CMPUT 274 FALL 2018                               |
|                                                   |
| Assignment 2 Part 2: Complete Implementation      |
|                                                   |
| This program allows two users to communicate      |
| through the serial port of two Arduinos. Once     |
| the computed shared secret key is entered into    |
| both Arduinos by the users.                       |
|                                                   |
| Typed characters will appear on both serial       |
| monitors simultaneously.                          |
|                                                   |
| Instructions on how the code is run can be found  |
| in the README.                                    |
|                                                   |
| Functions copied (and modified) from in class     |
| examples and code provided by assignment page:    |
|   genereate_key(); (bitwise_ops.cpp)              |
|   powModFast(); [powMod.cpp]                      |
|   wait_on_serial3(); (assignment page)            |
|   uint32_to_serial3(); (assignment page)          |
|   uint32_from_serial3(); (assignment page)        |
|   next_key(); (assignment page)                   |
|   encr(); (encrypt_decrypt.cpp)                   |
|                                                   |
| The printed text initially was copied to be       |
| identical to the demo video.                      |
+---------------------------------------------------+
*/

#include <Arduino.h>

const int idPin = 1; // Used for generation of random private key

uint32_t shkey;    // shared secret key
uint32_t otherkey; // other public key
uint32_t ownkey;   // own public key

const long int p = 2147483647;
const int g = 16807;

unsigned int send_count = 0;
unsigned int recv_count = 0;

const int serverPin = 13; // decides which Arduino is server/client

const char ACK = 'A'; // acknowledgement from server
const char CR = 'C';  // acknowledgement from client

/*
    generate_key() generates a 16 bit private key from the idPin
    uses the fluctuation of number read from pin to generate a random number
*/
uint32_t generate_key()
{
    long int key = 0; // output variable

    for (int i = 0; i < 32; i++) // 32 iterations -> 32 bits
    {
        // obtains least significant bit of analogRead
        // & is binary and operator. if LSB of analogRead is 0, gets 0. otherwise 1
        int bit = analogRead(idPin) & 1;

        // shorthand for key = key | (bit << i ); | is or operator
        // << is operator for binary shifting. 1 << 2 shifts 1 from 2^0 to 2^2
        key |= (bit << i);

        delay(50); // delay 50 ms
    }

    return key;
}

/*
	mulMod function calculates (a * b) mod m. This is done by the help
	of the worksheet given in class as well as the video resource shown.
	The general procedure is splitting "a" into its binary representation
	and performing long multiplication on each "bit" while take the mod.
	The running total is kept track and the modulo is taken at each iteration
*/
uint32_t mulMod(uint32_t a, uint32_t b, uint32_t m)
{
    // running "total"
    uint32_t ans = 0;

    b %= m;
    a %= m;

    // creates an unsigned integer of value 1 with 32 bits to shift a by
    uint32_t shift = 1;

    // iterates through 31 bits of the number "a" and shifts by the "i-th" term
    // to get the LSB and use that to perform long multiplication
    for (int i = 0; i < 31; i++)
    {
        // Goes through the binary representation of "a" and goes forward if
        // the LSB is a 1
        if ((a & (shift << i)))
        {
            // b mod m is added to the running total
            ans += (b % m);

            // the running total is moded with m
            ans %= m;
        }

        // b is doubled and then taked the mod of it
        b = (b * 2) % m;
    }

    return ans;
}

/*
	A faster method of taking a^b mod m (^ is exponentiation)
	Makes use of the mulMod() function to deal with the repeated 
	multiplication. 
	Source: eClass Lec 20, powmod.cpp (used and further improved on)
*/
uint32_t powModFast(uint32_t a, uint32_t b, uint32_t m)
{
    uint32_t result = 1 % m;
    uint32_t sqrVal = a % m;

    while (b > 0)
    {
        // evalutates to true if i'th bit of b is 1
        if (b & 1)
        {
            result = mulMod(result, sqrVal, m);
        }
        sqrVal = mulMod(sqrVal, sqrVal, m);
        b = (b >> 1);
    }

    return result % m;
}

/* 
    Waits for a certain number of bytes on Serial3 or timeout 
    @param nbytes: the number of bytes we want
    @param timeout: timeout period (ms); specifying a negative number
                   turns off timeouts (the function waits indefinitely
                   if timeouts are turned off).
    @return True if the required number of bytes have arrived.
    Source: eClass Assignment Page 
 */
bool wait_on_serial3(uint8_t nbytes, long timeout)
{
    // creates timer on how long to wait by finding current time and
    // adding the number of milliseconds to wait for
    unsigned long deadline = millis() + timeout;
    // wraparound not a problem

    while (Serial3.available() < nbytes &&
           (timeout < 0 || millis() < deadline))
    {
        delay(1); // be nice, no busy loop
    }
    return Serial3.available() >= nbytes;
}

/*
    Writes an uint32_t to Serial3, starting from the least-significant
    and finishing with the most significant byte. 
    Source: eClass Assignment 2 Page
*/
void uint32_to_serial3(uint32_t num)
{
    Serial3.write((char)(num >> 0));
    Serial3.write((char)(num >> 8));
    Serial3.write((char)(num >> 16));
    Serial3.write((char)(num >> 24));
}

/*
    Reads an uint32_t from Serial3, starting from the least-significant
    and finishing with the most significant byte. 
    Source: eClass Assignment 2 Page
*/
uint32_t uint32_from_serial3()
{
    uint32_t num = 0;
    num |= ((uint32_t)Serial3.read()) << 0;
    num |= ((uint32_t)Serial3.read()) << 8;
    num |= ((uint32_t)Serial3.read()) << 16;
    num |= ((uint32_t)Serial3.read()) << 24;

    return num;
}

/*
    server() function follows the flowchat shown in assignment
    page. Implements a finite state machine using a switch
    statement wrapped by a while loop. It first "Listen's" for
    the CR from the client. Once it recieves the character the Arduino
    moves to the "WaitingForKey" stage in which it waits for the ckey (otherkey)
    from the client, stores it then sends ACK and the server's ownkey. At 
    the "WaitforAck" stage it waits for the ACK character again from the client
    for it to move to the "Data Exchange" stage. 
*/
void server()
{
    bool sentownkey = false;

    // different server_states
    enum server_state
    {
        Listen,
        WaitingForKey,
        WaitForAck,
        DataExchange
    };

    // initiate server state to Listen mode
    server_state stage = Listen;

    while (true)
    {
        switch (stage)
        {

        // Listening Stage
        case Listen:

            Serial3.flush();
            while (Serial3.available() == 0)
            {
            }

            // wait until client sends CR.
            while (true)
            {
                // if client send CR, move to next stage
                if ((int)Serial3.read() == CR)
                {
                    stage = WaitingForKey;
                    break;
                }
            }
            break;

        // Waiting for Key stage
        case WaitingForKey:

            // if client sends 4 byte key within 1000 ms, store
            // and send ACK
            if (wait_on_serial3(4, 1000))
            {
                otherkey = uint32_from_serial3(); // store client key
                Serial3.write(ACK);

                if (not sentownkey)
                {
                    uint32_to_serial3(ownkey); // sends server key
                    sentownkey = true;
                }

                stage = WaitForAck;
                break;
            }
            else
            {
                // if no key is received, go back to Listening stage
                stage = Listen;
                break;
            }

        // Waiting for ACK stage
        case WaitForAck:

            if (wait_on_serial3(1, 1000))
            {

                uint8_t byte = Serial3.read();

                // if ACK is recieved move to DataExchange
                if ((int)byte == ACK)
                {
                    stage = DataExchange;
                    break;
                }
                // if CR is recieved move back to Wait for key stage
                else if ((int)byte == CR)
                {
                    stage = WaitingForKey;
                    break;
                }
            }
            // if nothing is recieved go back to Listening stage
            else
            {
                stage = Listen;
                break;
            }

        case DataExchange:
            break;

        // if something goes wrong, restart the program at Listen.
        default:
            stage = Listen;
            break;
        }
        // once server is in Data Exchange, flush buffer
        // and break outer while loop
        if (stage == DataExchange)
        {
            break;
        }
    }
}

/*
    client() function dictates what the client Arduino will do, following 
    the flowchart in the Assignment page. Implements Finite-State machine
    using switch statements. First it "Start's" and sends the CR char and 
    its ownkey. Then at the "Waiting for Ack" stage it waits for the server
    to send the ACK char and the server's key. Then moves to Data Exchange 
    stage. 
*/
void client()
{
    // client states for finite state-machine
    enum client_state
    {
        Start,
        WaitingForAck,
        DataExchange
    };

    // client initial state is Start
    client_state clientat = Start;

    while (true)
    {
        switch (clientat)
        {
        case Start:

            Serial3.write(CR);
            uint32_to_serial3(ownkey);

            // changes state to Waiting for Ack
            clientat = WaitingForAck;
            break;

        case WaitingForAck:

            if (wait_on_serial3(1, 1000) && (int)Serial3.read() == ACK)
            {
                while (true)
                {
                    // if server sent its 4 byte key, store it and break
                    // while loop
                    if (wait_on_serial3(4, 1000))
                    {
                        otherkey = uint32_from_serial3();
                        break;
                    }
                }

                // Sends ACK to server
                Serial3.write(ACK);

                // changes state to Data Exchange
                clientat = DataExchange;
                break;
            }

            // if no ACK from server, go back to listening stage
            else
            {
                clientat = Start;
                break;
            }

        case DataExchange:
            break;

        // if anything goes wrong, goes back to start stage
        default:
            clientat = Start;
            break;
        }

        // once at Data exchange state, flushs buffer and breaks outer
        // while loop
        if (clientat == DataExchange)
        {
            break;
        }
    }
}

/*
    handshake() uses the voltage of the server pin, will determine 
    which arduino is the server and which one is the client. 
    From there, the above server() and client() functions will run 
    going through the key exchange. 
*/
void handshake()
{
    // if serverPin (digital pin 13) is connected to +5V source, it is the server.
    // otherwise it is client.
    if (digitalRead(serverPin) == HIGH)
    {
        server();
    }

    else
    {
        client();
    }
}

/*
    initial setup for program: generates keys for both Arduino's and
    displays text in both Serial-mon. Calls upon handshake() function
    to initiate key exchange.
*/
void setup()
{

    init();

    // set idPin to INPUT and turn on the internal pullup resistor
    pinMode(idPin, INPUT);
    digitalWrite(idPin, HIGH);
    pinMode(serverPin, INPUT);

    // begin serial mons
    Serial.begin(9600);
    Serial3.begin(9600);

    Serial.println("Welcome to Arduino chat");

    // generates the random private key and public key
    uint32_t private_key = generate_key();
    ownkey = powModFast(g, private_key, p);
    Serial.print("Your private key is: ");
    Serial.println(private_key);

    Serial.print("Your public key is: ");
    Serial.println(ownkey);
    Serial.println();

    // initializes handshake() function to setup server/client arduinos
    handshake();

    shkey = powModFast(otherkey, private_key, p);
    Serial.print("Other key is: ");
    Serial.print(otherkey);
    Serial.println();

    Serial.print("Your shared key is: ");
    Serial.println(shkey);
    Serial.println();
    Serial.println();
}

/*
    Implements the Park-Miller algorithm with 32 bit integer arithmetic 
    @return ((current_key * 48271)) mod (2^31 - 1);
    This is linear congruential generator, based on the multiplicative
    group of integers modulo m = 2^31 - 1.
    The generator has a long period and it is relatively efficient.
    Most importantly, the generator's modulus is not a power of two
    (as is for the built-in rng), hence the keys mod 2^{s} cannot be obtained
    by using a key with s bits.
    Based on: http://www.firstpr.com.au/dsp/rand31/rand31-park-miller-carta.cc.txt
    Source: eClass Assignment 2 Page
 */
uint32_t next_key(uint32_t current_key)
{
    const uint32_t modulus = 0x7FFFFFFF; // 2^31-1
    const uint32_t consta = 48271;       // we use that consta<2^16
    uint32_t lo = consta * (current_key & 0xFFFF);
    uint32_t hi = consta * (current_key >> 16);
    lo += (hi & 0x7FFF) << 16;
    lo += hi >> 15;
    if (lo > modulus)
        lo -= modulus;
    return lo;
}

/*
    encr() is an encrypting/decrypting function that serves
    to encrypt outgoing messages and decrypts incoming messages
    encrypts/decrypts using xor operator
    Source: eClass encrypt_decrypt.cpp
*/
uint8_t encr(uint8_t byte, uint32_t key, unsigned int rounds)
{
    uint32_t current_key = key;
    for (int i = 0; i < (int)rounds; i++)
    {
        current_key = next_key(current_key);
    }

    uint8_t encrypted = byte ^ ((uint8_t)current_key);

    return (encrypted);
}

/*
    send() is the function that sends messages to the other arduino,
    also prints to own user's screen as well as makes sure to encrypt the
    text
*/
void send()
{
    // reads from user keyboard input on local serial
    uint8_t text = Serial.read();

    // prints to own serial what was typed
    Serial.write(text);

    // if carriage return was entered: it will encrypt the text and
    // print/write a newline character to both Serials

    // if there's a character available, write encrypted char to Serial 3
    Serial3.write(encr(text, shkey, send_count));
    send_count++;

    if ((int)text == 13)
    {
        Serial3.write(encr('\n', shkey, send_count));
        send_count++;
        Serial.println();
    }
}

/*
    receive() is the function that receives messages from the other arduino
    and decrypts as well.
*/
void receive()
{
    // reads from serial3 (other arduino) which will be in bytes
    uint8_t text = Serial3.read();

    // decrypts the text and stores in decr
    uint8_t decrypt = encr(text, shkey, recv_count);

    // writes the decrypted text into own serial and shows on screen
    Serial.write(decrypt);
    recv_count++;
}

/*
    main() function only calls setup() and a while loop that constantly
    keeps send() and receive() running
*/
int main()
{
    // calls setup() which does all the background work for the encryption
    setup();

    // clears Serial3 before proceeding to communication phase.
    while (Serial3.available() > 0)
    {
        Serial3.read();
    }

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

    Serial.flush();
    Serial3.flush();

    Serial.end();
    Serial3.end();

    return 0;
}