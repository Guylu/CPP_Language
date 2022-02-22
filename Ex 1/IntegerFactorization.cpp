#include <iostream>
#include "GFNumber.h"
#include <cassert>

/**
 * main program to run
 * @return SUCCESS/ FAIL
 */
int main()
{
    GFNumber g1, g2;
    // fail will hold if there was a failed attempt at creating a GFNumber
    bool fail = !(std::cin >> g1 >> g2);
    assert(!fail);
    std::cout << g1 + g2 << std::endl;
    std::cout << g1 - g2 << std::endl;
    std::cout << g2 - g1 << std::endl;
    std::cout << g1 * g2 << std::endl;

    g1.printFactors();
    g2.printFactors();

    return EXIT_SUCCESS;
}