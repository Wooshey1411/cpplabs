#include <iostream>

#include "BigInt.cpp"
int main(){
    BigInt long1 = BigInt("99989");
    BigInt long2 = BigInt("1");
    BigInt long3 = long1 + long2;
    for (int i = 0; i < long3.countOfDigits; ++i) {
        std::cout << long3.numberDigits[i] << ' ';
    }
    return 0;
}