
#include <iostream>

using namespace std;


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
    for (int i = 0; i < 31; i++)
    {
        // Goes thru the binary representation of "a" and goes forward if
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

uint32_t powModFast(uint32_t a, uint32_t b, uint32_t m)
{ /*
	A faster method of taking a^b mod m (^ is exponentiation)
	Makes use of the mulMod() function to deal with the repeated 
	multiplication. 
	Source: eClass Lec 20, powmod.cpp (used and further improved on)
    */
    uint32_t result = 1 % m;
    uint32_t sqrVal = a % m;

    while (b > 0)
    {
        // evalutates to true iff i'th bit of b is 1
        if (b & 1)
        {
            result = mulMod(result, sqrVal, m);
        }
        sqrVal = mulMod(sqrVal, sqrVal, m);
        b = (b >> 1);
    }

    return result % m;
}

int main()
{
    long int p = 2147483647;
    int g = 16807;

    uint32_t a = 4294945450;
    uint32_t A = powModFast(g,a,p);

    uint32_t B = 1897266531;

    uint32_t k = powModFast(B,a,p);

    cout << a << A << k << endl;

//744557920

    return 0;
}