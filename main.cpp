#include <iostream>

#include "BigInt.cpp"
int main(){
    BigInt long1 = BigInt("5");
    BigInt long2 = BigInt("1375");
    BigInt long3 = long2 * long1;
    if(!long3.isPositive())
        std::cout << "-";
    for (int i = 0; i < long3.countOfDigits(); ++i) {
        std::cout << long3.numberDigits()[i] << ' ';
    }

    return 0;
}