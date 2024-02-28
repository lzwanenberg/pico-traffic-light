#include <iostream>

#include "factorial/factorial.hpp"
#include "addition/addition.hpp"

int main() {
    int factorial_result = Factorial(4);
    int addition_result = Addition(2, 11);

    std::cout << "Hello, World!" <<std::endl;
    std::cout << "The factorial result of 4 is: " << factorial_result << std::endl;
    std::cout << "The addition result of 2 and 11 is: " << addition_result <<std::endl;

    return 0;
}
