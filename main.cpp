#include <iostream>

#include "BigInt.h"

int main(){
    BigInt long1 = BigInt("-643");
    BigInt long2 = BigInt("123");
    BigInt long3 = long1 * long2;
    if(!long3.isPositive())
        std::cout << "-";
    for (int i = 0; i < long3.countOfDigits(); ++i) {
        std::cout << long3.numberDigits()[i] << ' ';
    }

    return 0;
}