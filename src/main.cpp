#include <iostream>

#include "BigInt.h"

int main(){
    BigInt a(1234567);
    BigInt b(1111);
    a^=b;
    //std::cout << ~BigInt(-4) << " " << ~(-4);
    std::cout << a << " " << (1234567 ^ 1111);
}