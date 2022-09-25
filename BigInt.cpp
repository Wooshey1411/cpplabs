//
// Created by Admin on 13.09.2022.
//

#include "BigInt.h"

#include <memory>

inline long long base = 4294967296;

unsigned long* Resize(unsigned long* a, unsigned int *size, unsigned int newSize) // how protect ?
{
    auto* t = new unsigned long[newSize+1];
    std::copy(a, a + newSize, t);
    (*size) = newSize;
    delete[] a;
    return t;
}

unsigned long* shift(unsigned  long* a, unsigned int *size){
    auto* t = new unsigned long [*size + 1];
    for (int i = 0; i < *size; ++i)
        t[i+1] = a[i];
    t[0] = 0;
    (*size)++;
    delete[] a;
    return  t;
}

BigInt abs(BigInt num){
    if (num.isPositive())
        return num;
    else return -num;
}

BigInt::BigInt()
    :_isPositive(true), _countOfDigits(1){
    _numberDigits = new unsigned long[1];
    _numberDigits[0] = 0;
}

BigInt::BigInt(long num){
    _countOfDigits = 1;
    _numberDigits = new unsigned long[2];
    if(num < 0) {
        _isPositive = false;
        _numberDigits[0] = -num;
    }
    else {
        _isPositive = true;
        _numberDigits[0] = num;
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

    _numberDigits = new unsigned long[0];
    auto* newNumber = new unsigned char [num.size()];

    int currDigitPos = 0;
    _countOfDigits = 0;
    unsigned char shifter = 0;

    if (num[0] == '-') { // first char can be a symbol '-' if it right we need to shift string on 1 position left. It's doing by shifter
        shifter = 1;
        _isPositive = false;
    }
    else{
        _isPositive = true;
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
        _numberDigits = Resize(_numberDigits,&_countOfDigits,_countOfDigits + 1);
        _numberDigits[currDigitPos] = temp; // temp after temp=%base cannot be more than 2^32. I can take in unsigned long a number like temp
        currDigitPos++;
        length = countOfNewDigits;
        if(countOfNewDigits == 0)
            break;
    }

}

BigInt::BigInt(const BigInt& num)
    :_countOfDigits(num._countOfDigits), _isPositive(num._isPositive){
    this->_numberDigits = new unsigned long[num._countOfDigits];
    for (int i = 0; i < num._countOfDigits; ++i) {
        this->_numberDigits[i] = num._numberDigits[i];
    }
}

BigInt::BigInt(BigInt&& num) noexcept
    :_isPositive(false),_countOfDigits(0),_numberDigits(nullptr){
    _numberDigits = num._numberDigits;
    _countOfDigits = num._countOfDigits;
    _isPositive = num._isPositive;

    num._numberDigits = nullptr;
    num._countOfDigits = 0;
    num._isPositive = false;
}

BigInt& BigInt::operator=(const BigInt& num){
    if (this != &num) {
        this->_countOfDigits = num._countOfDigits;
        this->_isPositive = num._isPositive;
        this->_numberDigits = new unsigned long[num._countOfDigits];
        for (int i = 0; i < num._countOfDigits; ++i) {
            this->_numberDigits[i] = num._numberDigits[i];
        }
    }
    return *this;
}
BigInt& BigInt::operator=(BigInt&& num) noexcept{
    if (this != &num)
    {
        delete[] _numberDigits;
        _numberDigits = num._numberDigits;
        _isPositive = num._isPositive;
        _countOfDigits = num._countOfDigits;

        num._numberDigits = nullptr;
        num._countOfDigits = 0;
        num._isPositive = false;
    }
    return *this;
}

BigInt BigInt::operator+() const{
    return *this;
}
BigInt BigInt::operator-() const{
    BigInt x(*this);
    x._isPositive = !x._isPositive;
    return x;
}

BigInt& BigInt::operator+=(const BigInt& num){

    if(this->_isPositive && !num._isPositive) // a + -b = a - b
        return *this-=-num;
    if(!this->_isPositive && num._isPositive) // -a + b = -a + b = b - a = -a - (-b) = -a + b
        return *this-=-num;


    unsigned int maxLen;
    unsigned int minLen;
    const BigInt* maxNum;

    if(this->_countOfDigits >= num._countOfDigits){
        maxLen = this->_countOfDigits;
        minLen = num._countOfDigits;
        maxNum = this;
    } else{
        maxLen = num._countOfDigits;
        minLen = this->_countOfDigits;
        this->_numberDigits = Resize(this->_numberDigits, &this->_countOfDigits, num._countOfDigits);
        maxNum = &num;
    }

    unsigned char carry = 0;
    for (int i = 0; i < maxLen; ++i) {
        unsigned long long tmp;

        if(i < minLen)
            tmp = carry + (unsigned long long)this->_numberDigits[i] + (unsigned long long)num._numberDigits[i];
        else
            tmp = (unsigned long long)maxNum->_numberDigits[i] + carry;

        this->_numberDigits[i] = tmp % base;
        carry = tmp / base;

        if(i == maxLen - 1 && carry == 1) // 999 + 1 = 1000
        {
            this->_numberDigits = Resize(this->_numberDigits, &this->_countOfDigits, this->_countOfDigits + 1);
            this->_numberDigits[maxLen] = 1;
        }

    }
    return *this;
}
BigInt& BigInt::operator-=(const BigInt& num){
    if (this->_isPositive && !num._isPositive) // a - (-b) = a + b
        return *this+=-num;
    if (!this->_isPositive && num._isPositive) // -a - b = - (a+b) in += we have case of - (a+b)
        return *this +=-num;

    // 2 cases a - (+b) = a - b || -a - (-b) = b - a
    unsigned int minLen;
    unsigned int maxLen;

    const BigInt* firstNum;
    const BigInt* secondNum;
    if(abs(*this) >= abs(num)){ // |a| >= |b|. If b - a  => sign - (in this case *this have sign -) if a - b => sign + (in this case *this have sign +)
        minLen = num._countOfDigits;
        maxLen = this->_countOfDigits;
        firstNum = this;
        secondNum = &num;
    }else{ // |a| < |b|. If b - a => sign + (*this have sign -). If a - b => sign + (*this have sign +) -> we need resign this in both cases;
        minLen = this->_countOfDigits;
        maxLen = num._countOfDigits;
        firstNum = &num;
        secondNum = this;
        this->_isPositive = !this->_isPositive;
    }
    this->_numberDigits = Resize(this->_numberDigits, &this->_countOfDigits, maxLen);

    unsigned char debt = 0;
    for (int i = 0; i < maxLen; ++i) {
        long long tmp;
        if(i<minLen){
            tmp = (long long)firstNum->_numberDigits[i] - (long long)secondNum->_numberDigits[i] -debt;

            if (tmp < 0)
            {
                tmp +=base;
                debt = 1;
            }else
                debt = 0;

            this->_numberDigits[i] = tmp;
        }else{
            if(firstNum->_numberDigits[i] == 0 && debt == 1){ // 10000 - 1
                this->_numberDigits[i] = base - 1;
                debt = 1;
            }
            else if(firstNum->_numberDigits[i] != 0 && debt == 1) {
                this->_numberDigits[i] = firstNum->_numberDigits[i] - 1;
                debt = 0;
            } else this->_numberDigits[i] = firstNum->_numberDigits[i];
        }
    }

    unsigned int liveDigit = 0; // 67700 - 67695 = 5. only 1 digit is "live" his position is 0
    for (int i = 0; i < maxLen; ++i)
        if(this->_numberDigits[i] != 0)
            liveDigit = i;

    this->_numberDigits = Resize(this->_numberDigits, &this->_countOfDigits, liveDigit + 1);
    return *this;

}
BigInt& BigInt::operator*=(const BigInt& num){

    if (num == BigInt() || *this == BigInt()){ // a * 0 or 0 * a = 0
        *this = BigInt();
        return *this;
    }

    this->_isPositive = !(this->_isPositive ^ num._isPositive);

    if (abs(num) == BigInt(1)) // a * 1 = a
        return *this;
    if(abs(*this) == BigInt(1)) // 1 * b = b
        return (*this = num);

    BigInt result;
    BigInt temp; // 124*24 = 124*2 * 10 + 124*4 temp is result of 124*2 after 124*4 it's one of step for multiply in table
    temp._numberDigits = Resize(temp._numberDigits,&temp._countOfDigits, std::max(this->_countOfDigits,num._countOfDigits) + 1); // 999*9=8991 - 4 digits nax
    const BigInt* firstNum;
    const BigInt* secondNum;
    unsigned int minLen;
    unsigned int maxLen;
    if (this->_countOfDigits >= num._countOfDigits){
        firstNum = this;
        secondNum = &num;
        minLen = num._countOfDigits;
        maxLen = this->_countOfDigits;
    }else{
        firstNum = &num;
        secondNum = this;
        minLen = this->_countOfDigits;
        maxLen = num._countOfDigits;
    }
    unsigned long carry; // carry <= sizeof(unsigned long) - 1;
    for (long long i = minLen - 1; i >= 0 ; --i) {
        temp._countOfDigits = maxLen;
        carry = 0;
        for (int j = 0; j < maxLen; ++j) {
            unsigned long long tmp; // (2^32 - 1)^2 + 2^32 < 2^64;
            tmp = ((unsigned long long)secondNum->_numberDigits[i] * (unsigned long long)firstNum->_numberDigits[j]) + carry;
            temp._numberDigits[j] = tmp % base;
            carry = tmp / base;
          //  if (carry == 0 && j == maxLen - 1 && tmp != 0) // 99 + 1 = 100 where carry = 0
            //    carry = 1;
            if (j == maxLen - 1 && carry != 0 ) {
                temp._numberDigits[maxLen] = carry; // 12*9  = 108
                temp._countOfDigits++;
            }
        }
        result+=temp;
        if (i != 0)
            result._numberDigits = shift(result._numberDigits,&result._countOfDigits);
    }
    result._isPositive = this->_isPositive;
    *this = result;
    return *this;

}


bool BigInt::operator==(const BigInt& num) const
{
    if(this->_isPositive && !num._isPositive || !this->_isPositive && num._isPositive || this->_countOfDigits != num._countOfDigits) // different signs
        return false;
    for (int i = 0; i < this->_countOfDigits; ++i) {
        if(this->_numberDigits[i] != num._numberDigits[i])
            return false;
    }
    return true;
}
bool BigInt::operator!=(const BigInt& num) const{
    return !operator==(num);
}
bool BigInt::operator<(const BigInt& num) const{
    if (this->_isPositive && !num._isPositive || this->_countOfDigits > num._countOfDigits || operator==(num))
        return false;
    if(!this->_isPositive && num._isPositive || this->_countOfDigits < num._countOfDigits)
        return true;

    for (int i = 0; i < num._countOfDigits; ++i) { // both numbers have equal count of digits and sign

        if ((this->_numberDigits[i] < num._numberDigits[i] && num._isPositive) ||
                (this->_numberDigits[i] > num._numberDigits[i] && !num._isPositive)) // -111 > -222 but 111 < 222
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
    delete[] _numberDigits;
}
