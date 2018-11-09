// server.cpp
#include <Arduion.h>

void server()
{
    // FUNCTION HEADER //
    while (true)
    {

        if (Serial3.read() == CR)
        {
            // if received CR, wait for ckey

            /* NOTE:
            "One last detail is that when you read CR in listening mode
            of server, code will assume 4 bytes of ckey will follow.
            however, it may happen that the client didnt succeed in 
            sending these 4 bytes (bc it was reset in the meanwhile) or
            that CR was some leftover in the buffer. Thus, if the server
            cannot read 4 bytes in a short period of time, it should 
            return to listening state while it consumed all the bytes
            in the buffer."
            */

            if (wait_on_serial3(4, 1000))
            {
                // if ckey is received
                uint32_t ckey = uint32_from_serial3();
                Serial3.write(ACK);

                // powmod goes here
                uint_to_serial3(skey);

                // wait for ack
                while (true)
                {
                    if (wait_on_serial3(1, 1000))
                    {
                        CR = Serial3.read();
                    }
                }
            }
        }
    }
}