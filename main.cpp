#include <iostream>
#include "repl.hpp"

int main()
{
    std::cout << "Welcome to EagleCL! EagleCL is a programming language in an albanian syntax\n";
    std::cout << "Feel free to try the REPL!\n";
    repl::start(std::cin, std::cout);
    return 0;
}