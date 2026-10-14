#include <iostream>
#include <cassert>
#include "shared/types.hpp"

int main() {
    assert(std::string(the_stations::Version::STRING) == "1.0.0");
    std::cout << "Test Sanity Passed!" << std::endl;
    return 0;
}
