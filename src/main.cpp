#include <iostream>

#include "factorial/factorial.hpp"
#include "addition/addition.hpp"
#include "substraction/substraction.hpp"

int main() {
    int factorial_result = Factorial(4);
    int addition_result = Addition(2, 11);
    int substraction_result = Substraction(11, 2);

    std::cout << "Hello, World!" <<std::endl;
    std::cout << "The factorial result of 4 is: " << factorial_result << std::endl;
    std::cout << "The addition result of 2 and 11 is: " << addition_result <<std::endl;
    std::cout << "The substraction result of 11 and 2 is: " << substraction_result <<std::endl;

    return 0;
}
