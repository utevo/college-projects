#include "BigNumber.h"

#include <iostream>
#include <climits>


int main()
{
    BigNumber x;
    BigNumber y;
    std::string tempString;

    char mathSign;

    std::cout << "Enter mathematical sign {'+', '-', '*'. '/'} (or '#' to exit) : ";
    std::cin >> mathSign;

    while (mathSign != '#')
    {

        std::cout << "Enter number 1: ";
        std::cin >> tempString;
        x = tempString;

        std::cout << "Enter number 2: ";
        std::cin >> tempString;
        y = tempString;


        std::cout << std::endl;

        switch (mathSign)
        {
            case '+':
                std::cout << x + y;
            break;

            case '-':
                std::cout << x - y;
            break;

            case '*':
                std::cout << x * y;
            break;

            case '/':
                std::cout << x / y;
            break;

            default:
                throw std::invalid_argument("Mathematical sign mus be from {'+', '-', '*'. '/'}.");
            break;
        }

        std::cout << "\n\n";

        std::cout << "Enter mathematical sign {'+', '-', '*'. '/'} (or '#' to exit) : ";
        std::cin >> mathSign;

    }

    return 0;
}
