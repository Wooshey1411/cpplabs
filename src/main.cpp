#include <iostream>

#include "BigInt.h"

int main(){
    BigInt a(123);
    BigInt b(123456);
    b = ~a;
    std::cout << (~a);
    return 0;
}