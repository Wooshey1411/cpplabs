//
// Created by Admin on 13.09.2022.
//

#include "BigInt.h"

#include <cmath>

inline unsigned long long base = 10;

BigInt::BigInt()
    :isPositive(true), countOfDigits(1){
    numberDigits = new unsigned long[1];
    numberDigits[0] = 0;
}

BigInt::BigInt(long num){
    countOfDigits = 1;
    numberDigits = new unsigned long[2];
    if(num < 0) {
        isPositive = false;
        numberDigits[0] = -num;
    }
    else {
        isPositive = true;
        numberDigits[0] = num;
    }
}

BigInt::BigInt(const std::string& num) {
    size_t length = num.size();

    if(!isdigit(num[0]) && num[0] != '-'){ // check input data on correct of value
        throw std::runtime_error("Error - wrong symbol in string number");
    }
    for (int i = 1; i < length; ++i) {
        if(!std::isdigit(num[i])){
            throw std::runtime_error("Error - wrong symbol in string number");
         }
    }

    numberDigits = new unsigned long[length/(size_t)floor(log10((double)base)) + 1]; // realloc ?
    auto* newNumber = new unsigned char [num.size()];
    int currDigitPos = 0;
    unsigned char shifter = 0;

    if (num[0] == '-') { // first char can be a symbol '-' if it right we need to shift string on 1 position left. It's doing by shifter
        shifter = 1;
        isPositive = false;
    }
    else{
        isPositive = true;
    }

    for (int i = 0+shifter; i < length; ++i)
        newNumber[i-shifter] = (num[i] - '0');
    length-=shifter;

    while (true){ // building number in base 2^32 by division into a column
        unsigned long long temp = 0;
        int countOfNewDigits = 0;
        bool takeDigits = false;

        for (int i = 0; i < length; ++i){
            temp = 10*temp + (newNumber[i]);
            if (temp >= base){
                takeDigits = true;
                newNumber[countOfNewDigits] = (unsigned char)(temp/base);
                temp%=base;
                countOfNewDigits++;
            }
            else if (takeDigits){
                newNumber[countOfNewDigits] = 0;
                countOfNewDigits++;
            }
        }
        numberDigits[currDigitPos] = temp; // temp after temp=%base cannot be more than 2^32. I can take in unsigned long a number like temp
        currDigitPos++;
        length = countOfNewDigits;
        if(countOfNewDigits == 0)
            break;
    }
    countOfDigits = currDigitPos;

    /*for (int i = 0; i < countOfDigits / 2; ++i) {
        unsigned long temp = numberDigits[countOfDigits-i-1];
        numberDigits[countOfDigits-i-1] = numberDigits[i];
        numberDigits[i]=temp;
    }*/
    delete[] newNumber;
}

BigInt::BigInt(const BigInt& num)
    :countOfDigits(num.countOfDigits), isPositive(num.isPositive){
    this->numberDigits = new unsigned long[num.countOfDigits];
    for (int i = 0; i < num.countOfDigits; ++i) {
        this->numberDigits[i] = num.numberDigits[i];
    }
}

BigInt::BigInt(BigInt&& num) noexcept
    :isPositive(false),countOfDigits(0),numberDigits(nullptr){
    numberDigits = num.numberDigits;
    countOfDigits = num.countOfDigits;
    isPositive = num.isPositive;

    num.numberDigits = nullptr;
    num.countOfDigits = 0;
    num.isPositive = false;
}

BigInt& BigInt::operator=(const BigInt& num){
    if (this != &num) {
        this->countOfDigits = num.countOfDigits;
        this->isPositive = num.isPositive;
        this->numberDigits = new unsigned long[num.countOfDigits];
        for (int i = 0; i < num.countOfDigits; ++i) {
            this->numberDigits[i] = num.numberDigits[i];
        }
    }
    return *this;
}
BigInt& BigInt::operator=(BigInt&& num) noexcept{
    if (this != &num)
    {
        delete[] numberDigits;
        numberDigits = num.numberDigits;
        isPositive = num.isPositive;
        countOfDigits = num.countOfDigits;

        num.numberDigits = nullptr;
        num.countOfDigits = 0;
        num.isPositive = false;
    }
    return *this;
}

BigInt BigInt::operator+() const{
    return *this;
}
BigInt BigInt::operator-() const{
    BigInt x(*this);
    x.isPositive = !x.isPositive;
    return x;
}

BigInt BigInt::operator+(const BigInt& num){
    int maxLen = std::max(this->countOfDigits,num.countOfDigits);
    int minLen = std::min(this->countOfDigits,num.countOfDigits);
    BigInt x;
    if(this->countOfDigits > num.countOfDigits)
        x = *this;
    else
        x = num;


    unsigned char carry = 0;

    for (int i = 0; i < maxLen; ++i) {
        if (i < minLen){
            if ((unsigned long long)this->numberDigits[i] + (unsigned long long)num.numberDigits[i] + carry >= base){
                x.numberDigits[i] = this->numberDigits[i] + num.numberDigits[i] + carry - base;
                carry = 1;
            } else{
                x.numberDigits[i] = this->numberDigits[i] + num.numberDigits[i] + carry;
                carry = 0;
            }
        }
        else if((unsigned long long)x.numberDigits[i]+carry >= base){
            x.numberDigits[i] = 0;
            carry = 1;
        }else {
            x.numberDigits[i]+=carry;
            carry = 0;
        }
    }
    if (carry == 1) {
        x.numberDigits[maxLen] = 1;
        x.countOfDigits++;
    }
    return x;

}
BigInt BigInt::operator-(const BigInt& num){

}


bool BigInt::operator==(const BigInt& num) const
{
    if(this->isPositive && !num.isPositive || !this->isPositive && num.isPositive || this->countOfDigits != num.countOfDigits) // different signs
        return false;
    for (int i = 0; i < this->countOfDigits; ++i) {
        if(this->numberDigits[i] != num.numberDigits[i])
            return false;
    }
    return true;
}
bool BigInt::operator!=(const BigInt& num) const{
    return !operator==(num);
}
bool BigInt::operator<(const BigInt& num) const{
    if (this->isPositive && !num.isPositive || this->countOfDigits > num.countOfDigits || operator==(num))
        return false;
    if(!this->isPositive && num.isPositive || this->countOfDigits < num.countOfDigits)
        return true;

    for (int i = 0; i < num.countOfDigits; ++i) { // both numbers have equal count of digits and sign

        if ((this->numberDigits[i] < num.numberDigits[i] && num.isPositive) ||
                (this->numberDigits[i] > num.numberDigits[i] && !num.isPositive)) // -111 > -222 but 111 < 222
            return true;
    }

    return false;
}
bool BigInt::operator>(const BigInt& num) const{ // > ~ not < and not == because !< = >=
    if(!operator<(num) && operator!=(num))
        return true;
    return false;
}
bool BigInt::operator<=(const BigInt& num) const{
    if (!operator>(num))
        return true;
    return false;
}
bool BigInt::operator>=(const BigInt& num) const
{
    if(!operator<(num))
        return true;
    return false;
}

BigInt::~BigInt()
{
    delete[] numberDigits;
}
