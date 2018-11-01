/*
    Name: Cynthia Li
    ID: 1529078
    CMPUT 274 FALL 2018
    
    WEEKLY EXERCISE 5 counting lights
    This program counts in binary. Each light represents a bit with the 
    most significant bit being the leftmost. A 1 is represented by a turned on
    LED, while a 0 is the opposite.
    
   Starting from when all the LEDs are off, corresponding to a count of 0, each press
   of the increment button will add one to the count and the program will display the
   binary representative of the number.

*/

#include <Arduino.h>

const int LED_Count = 5;

// Pin Declarations
const int Meter[LED_Count] = {9, 10, 11, 12, 13};

// configures the weight of LEDs and max decimal thats representable
const int LEDWeight[LED_Count] = {1, 2, 4, 8, 16};
const int Max = 31;

// declares increment and decrement pins
const int IncrPin = 6;
const int DecrPin = 7;

// initializes the counting number, which will be modified by increment and decrement buttons
int Count = 0;

void setup()
{
    // sets up arduino and its pins

    init();
    // Serial.begin(9600);

    // configures LEDs to be digital outputs
    for (int i = 0; i < LED_Count; i++)
    {
        pinMode(Meter[i], OUTPUT);
    }
    // Serial.println("pinmode setup");

    // configure button pins to be inputs (digital)
    pinMode(IncrPin, INPUT);
    digitalWrite(IncrPin, HIGH);
    pinMode(DecrPin, INPUT);
    digitalWrite(DecrPin, HIGH);
    // Serial.println("setup2");
}

void lightsoff()
{
    // turns off all lights

    for (int i = 0; i < LED_Count; i++)
    {
        digitalWrite(Meter[i], LOW);
    }

    // Serial.println("lights off");
}

void lightson()
{
    // turns on all lights

    for (int i = 0; i < LED_Count; i++)
    {
        digitalWrite(Meter[i], HIGH);
    }
}

void multilight()
{
    // turns on multiple lights depending on count

    lightsoff();
    // Serial.println("multilight lights off");
    int newcount = Count;

    // for each LED, check if current count can be created as a sum for the numbers
    // if so, turns on corresponding light and subtracts the weight from the count
    // continues until count reaches 0 and is fully represented
    for (int i = LED_Count-1; i > -1; i--)
    {
        if (newcount - LEDWeight[i] >= 0)
        {
            digitalWrite(Meter[i], HIGH);

            // Serial.println(newcount);
            // Serial.println(LEDWeight[i]);
            // Serial.println(i);
            newcount = newcount - LEDWeight[i];
            // Serial.println(newcount);
            
        }
    }
}

void light()
{
    // checks if count is currently in one of three easily treatable cases: equal to a LED's weighed number,
    // is in overflow, or negative. otherwise, count is passed to multilight function.

    // boolean flag for when only a single LED needs to be on
    bool single = false;

    // goes through all the LEDs to check if any are equal to count. if so, turns on corresponding weighted
    // LED and turns off flag for multiple lights
    for (int i = 0; i < LED_Count; i++)
    {
        if (Count == LEDWeight[i])
        {
            // if count is at a number that equals the weight of one LED, turn off all lights and turn on
            // that LED
            lightsoff();
            // Serial.println("count equal weight");
            digitalWrite(Meter[i], HIGH);
            single = true;
        }
    }

    if (!single)
    {
        // if not only one LED is needed, calls for multiple LEDs to be turned on
        multilight();
    }

    if (Count > Max)
    {
        // if current number of count exceeds the number that can be represented by LEDs,
        // reset counter
        lightsoff();
        Count = 0;
        // Serial.println("Count reset");
    }

    if (Count < 0)
    {
        // if count is negative, turn off all lights
        lightsoff();
        Count = 0;
    }
}

void increment()
{
    // Increments count and passes to light function to display binary representation

    ++Count;

    // Serial.println(Count);

    light();
}

void decrement()
{
    // Decrements count and passes to light function to display appropriate binary representation

    --Count;

    // Serial.println(Count);

    light();
}

int main()
{
    // main function. calls setup() function to setup pins and buttons
    setup();

    // Serial.print("done setup");

    // initializes the state of the increment and decrement buttons
    int IncrVal;
    int DecrVal;
    // Serial.println("initiated Incr/DecrVal pins");

    // boolean flags for whether the button is being pressed
    bool Ipress = false;
    bool Dpress = false;

    while (true)
    {
        // maintain funtionality

        // reads current state of buttons. high = not pressed
        IncrVal = digitalRead(IncrPin);
        DecrVal = digitalRead(DecrPin);

        // if one button is pressed and it wasnt held down before, increment/decrement appropriately
        if (IncrVal == LOW && DecrVal == HIGH && !Ipress)
        {
            Ipress = true;
            increment();
            // Serial.println("Incr pressed");
        }
        if (DecrVal == LOW && IncrVal == HIGH && !Dpress)
        {
            Dpress = true;
            decrement();
            // Serial.println("Decr pressed");
        }

        // resets booleans glafs if button is let go
        if (IncrVal == HIGH)
        {
            Ipress = false;
        }
        if (DecrVal == HIGH)
        {
            Dpress = false;
        }
    }

    // Serial.flush();

    return 0;
}