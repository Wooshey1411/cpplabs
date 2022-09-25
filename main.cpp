#include <iostream>

#include "BigInt.cpp"
int main(){
    BigInt long1 = BigInt("22813371488360420146322246913579753086422281337148836042014632224691357975308642");
    BigInt long2 = BigInt("22813371488360420146322246913579753086422281337148836042014632224691357975308642123456789");
    long2*=long1;
    if(!long2.isPositive())
        std::cout << "-";
    for (int i = 0; i < long2.countOfDigits(); ++i) {
        std::cout << long2.numberDigits()[i] << ' ';
    }



    std::cout << "\n\n";
    BigInt long3 = BigInt("520449918665936131523747374278587628027116917023067758115082997558989233514021810586372643790667416842945332704790775012662622332367421722378066317656731565310225270538");
    for (int i = 0; i < long3.countOfDigits(); ++i) {
        std::cout << long3.numberDigits()[i] << ' ';
    }


    return 0;
}