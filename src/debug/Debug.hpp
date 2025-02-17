#pragma once

#include <iostream>

class Debug {
public:
    template <typename T, typename U>
    static void print(const T& var1, const U& var2){
        std::cout << var1 << ", " << var2 << std::endl;
    };
};

