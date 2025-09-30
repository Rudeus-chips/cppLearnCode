// main
#include "return_explicit.h"
#include <iostream>
using namespace ReturnExplicit;

int main() {
    vector<int> v(5, 1);
    A a(5, 4.0);
    
    std::cout << a.getData2() << std::endl;

    std::cout << "over" << std::endl;
    return 0;
}