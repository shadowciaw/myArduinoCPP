unsigned int mulMod(unsigned int a, unsigned int b, unsigned int m)
{
    /*
	mulMod function calculates (a * b) mod m. This is done by the help
	of the worksheet given in class as well as the video resource shown.
	The general procedure is splitting "a" into its binary representation
	and performing long multiplication on each "bit" while take the mod.
	The running total is kept track and the modulo is taken at each iteration
    */
    // running "total"
    unsigned int ans = 0;
    b %= m;
    a %= m;
    // creates an unsigned integer of value 1 with 32 bits to shift a by
    unsigned int shift = 1;
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

unsigned int powModFast(unsigned int a, unsigned int b, unsigned int m)
{ /*
	A faster method of taking a^b mod m (^ is exponentiation)
	Makes use of the mulMod() function to deal with the repeated 
	multiplication. 
	Source: eClass Lec 20, powmod.cpp (used and further improved on)
    */
    unsigned int result = 1 % m;
    unsigned int sqrVal = a % m;

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

    int a = 25962;
    return 0;
}