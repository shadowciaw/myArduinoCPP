/*
+---------------------------------------------------+
| NAME: Cynthia Li, Cyrus Diego                     |
| ID: 1529078, 1528911                              |
| CMPUT 274 FALL 2018                               |
|                                                   |
| Assignment 2 Part 1: Proof of Concept             |
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
| examples:                                         |
|   readString(); (read_int.cpp)                    |
|   readUnsigned16(); (read_int.cpp)                |
|   genereate_key(); (bitwise_ops.cpp)              |
|   powMod(); (diffie-hellman-prelim.cpp)           |
|   wait_on_serial3(); (assignment page)            |
|   uint32_to_serial3(); (assignment page)          |
|   uint32_from_serial3(); (assignment page)        |
|   next_key(); (assignment page)                   |
|   encr(); (encrypt_decrypt.cpp)                   |
|   powModFast(); (powMod.cpp)                      |
|                                                   |
| The printed text initially was copied to be       |
| identical to the demo video.                      |
+---------------------------------------------------+
*/

#include <Arduino.h>

const int idPin = 1; // Used for generation of random private key

uint32_t skey; // shared secret key
uint32_t ckey; // own private key

unsigned int send_count = 0;
unsigned int recv_count = 0;

const int serverPin = 13; // decides which Arduino is server/client

uint8_t ACK = 'A'; // acknowledgement from server
uint8_t CR = 'C';  // acknowledgement from client

int generate_key()
{
    /*
    generate_key() generates a 16 bit private key from the idPin
    uses the fluctuation of number read from pin to generate a random number
    */

    long int key = 0; // output variable

    for (int i = 0; i < 32; i++) // 16 iteratiosn -> 16 bits
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

uint32_t mulMod(uint32_t a, uint32_t b, uint32_t m)
{
    /*
	mulMod function calculates (a * b) mod m. This is done by the help
	of the worksheet given in class as well as the video resource shown.
	The general procedure is splitting "a" into its binary representation
	and performing long multiplication on each "bit" while take the mod.
	The running total is kept track and the modulo is taken at each iteration
    */
	// running "total"
    uint32_t ans = 0; 
    b %= m;
    a %= m;
	// creates an unsigned integer of value 1 with 32 bits to shift a by
    uint32_t shift = 1;
	// iterates thru 31 bits of the number "a" and shifts by the "i-th" term
	// to get the LSB and use that to perform long multiplication 
    for(int i = 0; i < 31; i++)
    {
		// Goes thru the binary representation of "a" and goes forward if 
		// the LSB is a 1
        if ((a & (shift << i)))
        {	
			// b mod m is added to the running total
            ans += (b % m);
			// the running total is moded with m 
            ans %=  m; 
        }
		// b is doubled and then taked the mod of it 
        b = (b * 2) % m;
    }
    return ans;

}

uint32_t powModFast(uint32_t a, uint32_t b, uint32_t m) 
{   /*
	A faster method of taking a^b mod m (^ is exponentiation)
	Makes use of the mulMod() function to deal with the repeated 
	multiplication. 
	Source: eClass Lec 20, powmod.cpp (used and further improved on)
    */
	uint32_t result = 1%m;
	uint32_t sqrVal = a%m; 

	while (b > 0) 
	{
		// evalutates to true iff i'th bit of b is 1
		if (b & 1) 
		{ 
			result = mulMod(result,sqrVal,m);
		}
		sqrVal = mulMod(sqrVal,sqrVal,m);
		b = (b >> 1);
	}

	return result % m;
}

/** Waits for a certain number of bytes on Serial3 or timeout 
 * @param nbytes: the number of bytes we want
 * @param timeout: timeout period (ms); specifying a negative number
 *                turns off timeouts (the function waits indefinitely
 *                if timeouts are turned off).
 * @return True if the required number of bytes have arrived.
 */
bool wait_on_serial3(uint8_t nbytes, long timeout)
{
    unsigned long deadline = millis() + timeout;
    //wraparound not a problem
    while (Serial3.available() < nbytes &&
           (timeout < 0 || millis() < deadline))
    {
        delay(1); // be nice, no busy loop
    }
    return Serial3.available() >= nbytes;
}

/** Writes an uint32_t to Serial3, starting from the least-significant
 * and finishing with the most significant byte. 
 */
void uint32_to_serial3(uint32_t num)
{
    Serial3.write((char)(num >> 0));
    Serial3.write((char)(num >> 8));
    Serial3.write((char)(num >> 16));
    Serial3.write((char)(num >> 24));
}

/** Reads an uint32_t from Serial3, starting from the least-significant
 * and finishing with the most significant byte. 
 */
uint32_t uint32_from_serial3()
{
    uint32_t num = 0;
    num = num | ((uint32_t)Serial3.read()) << 0;
    num = num | ((uint32_t)Serial3.read()) << 8;
    num = num | ((uint32_t)Serial3.read()) << 16;
    num = num | ((uint32_t)Serial3.read()) << 24;
    return num;
}

void server()
{
    // FUNCTION HEADER //

    bool gotCR = false;
    while (true)
    {
        if (wait_on_serial3(1, 1000))
        {
            gotCR = true;
            break;
        }
    }

    /*
        int B = readUnsigned32();
        // B is the public key of the other user.
        // might need to change this up? not sure if it would overflow.
    */

    if (gotCR)
    {
        while (true)
        {
            if (wait_on_serial3(4, 1000))
            {
                uint32_t ckey = uint32_from_serial3();
            }
            else
                ()
                {
                    gotCR = false;
                    break;
                }
        }
    }
}

void client()
{
    // FUNCTION HEADER //

    while (true)
    {
        // send CR(ckey); 5 bytes long: 'C' followed by 4 bytes of public key
        Serial3.write(CR);
        Serial3.write(ckey);

        if (wait_on_serial3(1, 1000) && (Serial3.read() == ACK) )
        {
            // if required number of bytes has arrived
            // the 1 byte should be ACK ('A')
            uint8_t check = Serial3.read();
            if ((int) check == ACK ) {
                break;
            }
        }
    }

    // receive shared key

    skey = uint32_from_serial3();

    // send ACK
    Serial3.write(ACK);
}

void handshake()
{
    // FUNCTION HEADER //

    if (serverPin == HIGH)
    {
        // if serverPin (digital pin 13) is connected to +5V source, it is the server.
        // otherwise it is client.

        server();
    }

    if (serverPin == LOW)
    {
        client();
    }
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
    pinMode(serverPin, INPUT);

    // begin serial mons
    Serial.begin(9600);
    Serial3.begin(9600);

    Serial.println("Welcome to Arduino chat");

    // generates the random private key and public key
    uint16_t private_key = generate_key();
    long int p = 2147483647;
    int g = 16807;
    uint32_t ckey = powModFast(g, private_key, p);

    Serial.print("Your public key is: ");
    Serial.println(ckey);
    Serial.println();

    /* TO DO: HANDSHAKING:
        CLIENT: keeps sending CONNECTION REQUESTS to server
        SERVER: when server captures one of these requests, acknowledge receipt of message
            ( so client knows partner is there before moving on )
        CLIENT: sends acknowledgement of acknowledgement
            required bc server may have received multiple connection requests that must be discarded
            before it moves to the data exchange phase.
            otherwise these requests will show up at beginning of data exchange as messages from client
        SERVER: while waiting for ack of ack, consumes all outstanding connection requests
            so when ack arrives, it can move only data exchange phase knowing all connection requests have 
            been consumed.
    */

    handshake();

    Serial.print("Your shared key is: ");
    Serial.println(skey);
    Serial.println();
    Serial.println();
}

/** Implements the Park-Miller algorithm with 32 bit integer arithmetic 
 * @return ((current_key * 48271)) mod (2^31 - 1);
 * This is linear congruential generator, based on the multiplicative
 * group of integers modulo m = 2^31 - 1.
 * The generator has a long period and it is relatively efficient.
 * Most importantly, the generator's modulus is not a power of two
 * (as is for the built-in rng),
 * hence the keys mod 2^{s} cannot be obtained
 * by using a key with s bits.
 * Based on:
 * http://www.firstpr.com.au/dsp/rand31/rand31-park-miller-carta.cc.txt
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

uint8_t encr(uint8_t byte, uint32_t key, unsigned int rounds)
{
    /*
    encr() is an encrypting/decrypting function that serves
    to encrypt outgoing messages and decrypts incoming messages
    */

    // encrypts/decrypts using xor operator

    uint32_t current_key = key;

    for (int i = 0; i < rounds; i++)
    {
        current_key = next_key(current_key)
    }

    uint8_t encrypted = byte ^ ((uint8_t)current_key);

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

    // if there's a character available, write encrypted char to Serial 3
    Serial3.write(encr(text, skey, send_count));
    send_count++;

    if ((int)text == 13)
    {
        Serial3.write(encr('\n', skey, send_count));
        send_count++;
        Serial.println();
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
    uint8_t decrypt = encr(text, skey, recv_count);

    // writes the decrypted text into own serial and shows on screen
    Serial.write(decrypt);
    recv_count++;
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

    Serial.flush();
    Serial3.flush();

    Serial.end();
    Serial3.end();

    return 0;
}