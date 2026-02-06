#include <iostream>
#include "math_utils.hpp"

int main() {
    std::cout << "Math Library Test" << std::endl;
    std::cout << "5 + 3 = " << math::add(5, 3) << std::endl;
    std::cout << "5 * 3 = " << math::multiply(5, 3) << std::endl;
    return 0;
}
