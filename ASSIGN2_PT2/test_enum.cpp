#include <iostream>

int main()
{
    enum color
    {
        red,
        green,
        blue
    };
    color G = green;

    while (true)
    {

        switch (G)
        {
        case red:
            std::cout << "red\n";
            G = green;
        case green:
            std::cout << "green\n";
            G = blue;
        case blue:
            std::cout << "blue\n";
            G = red;
        }
    }
    return 0;
}