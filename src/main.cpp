#include <iostream>

#include "BigInt.h"

int main(){
    BigInt a(1234567);
    BigInt b(1111);
    a^=b;
    std::cout << ~BigInt(123) << " " << ~(123);
    //std::cout << a << " " << (123 ^ 13);
}