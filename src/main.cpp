#include <iostream>

#include "BigInt.h"

int main(){
    BigInt a(123);
    uint8_t b = 123;
    b = ~b;
    //unsigned int b = ~(-123);
    std::cout << (~a) << " " <<int(b)<<"\n";
//    BigInt b;
//    b = ~a;
//    std::cin >> a >> b;
//    std:: cout << a << " " << b;
//    return 0;
}