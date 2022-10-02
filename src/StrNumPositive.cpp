#include "StrNumPositive.h"

const unsigned short base = 10;

static unsigned char* resize(unsigned char* a, unsigned int *size, unsigned int newSize){
    auto* t = new unsigned char[newSize+1];
    std::copy(a, a + newSize, t);
    (*size) = newSize;
    delete[] a;
    return t;
}

static unsigned char* shift(const unsigned char* a, unsigned int *size){
    auto* t = new unsigned char [*size + 1];
    for (unsigned long i = 0; i < *size; ++i)
        t[i+1] = a[i];
    t[0] = 0;
    (*size)++;
    delete[] a;
    return  t;
}

StrNumPositive::StrNumPositive()
:_digits(new unsigned char[1]),_length(1){
    _digits[0] = 0;
}

StrNumPositive::StrNumPositive(unsigned long num)
:_digits(new unsigned char[15]){
    unsigned long temp = num;
    unsigned int len = 0;
    while(true){
        _digits[len] = temp % base;
        temp/=base;
        len++;
        if(temp == 0)
            break;
    }
    _length = 15;
    _digits = resize(_digits, &_length,len);
}

StrNumPositive::StrNumPositive(const std::string& str){
    size_t len = str.length();
    if (len == 0)
        throw std::runtime_error("Error - wrong symbol in string number");
    for (size_t i = 0; i < len; ++i)
        if(!std::isdigit(str[i]))
            throw std::runtime_error("Error - wrong symbol in string number");
    _length = static_cast<unsigned int>(len);
    _digits = new unsigned char[_length];
    for (unsigned int i = 0; i < _length; ++i)
        _digits[i] = str[_length - i - 1] - '0';
}

StrNumPositive::StrNumPositive(const StrNumPositive& num)
:_length(num._length){
    this->_digits = new unsigned char[num._length];
    for (unsigned int i = 0; i < num._length; ++i)
        this->_digits[i] = num._digits[i];
}

StrNumPositive& StrNumPositive::operator=(const StrNumPositive& num){
    this->_length = num._length;
    delete[] this->_digits;
    this->_digits = new unsigned char[num._length];
    for (unsigned int i = 0; i < num._length; ++i)
        this->_digits[i] = num._digits[i];
    return *this;
}
StrNumPositive operator+(const StrNumPositive& num1, const StrNumPositive& num2){
    StrNumPositive x(num1);
    x+=num2;
    return x;
}
StrNumPositive operator*(const StrNumPositive& num1, const StrNumPositive& num2){
    StrNumPositive x(num1);
    x*=num2;
    return x;
}

StrNumPositive& StrNumPositive::operator+=(const StrNumPositive& num){

    unsigned int maxLen;
    unsigned int minLen;
    const StrNumPositive* maxNum;

    if(this->_length >= num._length){
        maxLen = this->_length;
        minLen = num._length;
        maxNum = this;
    } else{
        maxLen = num._length;
        minLen = this->_length;
        this->_digits = resize(this->_digits, &this->_length, num._length);
        maxNum = &num;
    }

    unsigned char carry = 0;
    for (unsigned int i = 0; i < maxLen; ++i) {
        unsigned short tmp;

        if(i < minLen)
            tmp = carry + static_cast<unsigned short>(this->_digits[i]) + static_cast<unsigned short>(num._digits[i]);
        else
            tmp = static_cast<unsigned short>(maxNum->_digits[i]) + carry;

        this->_digits[i] = tmp % base; // tmp % base <= 9
        carry = tmp / base; // carry is [0:1];

        if(i == maxLen - 1 && carry == 1) // 999 + 1 = 1000
        {
            this->_digits = resize(this->_digits, &this->_length, this->_length + 1);
            this->_digits[maxLen] = 1;
        }

    }
    return *this;

}
StrNumPositive& StrNumPositive::operator*=(const StrNumPositive& num){

    if (num == StrNumPositive() || *this == StrNumPositive()){ // a * 0 or 0 * a = 0
        *this = StrNumPositive();
        return *this;
    }

    if (num == StrNumPositive(1)) // a * 1 = a
        return *this;
    if(*this == StrNumPositive(1)) // 1 * b = b
        return (*this = num);

    StrNumPositive result;
    StrNumPositive temp; // 124*24 = 124*2 * 10 + 124*4 temp is result of 124*2 after 124*4 it's one of step for multiply in table
    temp._digits = resize(temp._digits,&temp._length, std::max(this->_length,num._length) + 1); // 999*9=8991 - 4 digits nax
    const StrNumPositive* firstNum;
    const StrNumPositive* secondNum;
    unsigned int minLen;
    unsigned int maxLen;
    if (this->_length >= num._length){
        firstNum = this;
        secondNum = &num;
        minLen = num._length;
        maxLen = this->_length;
    }else{
        firstNum = &num;
        secondNum = this;
        minLen = this->_length;
        maxLen = num._length;
    }

    unsigned short carry; // carry <= sizeof(unsigned short) - 1;
    for (long long i = minLen - 1; i >= 0 ; --i) {
        temp._length = maxLen;
        carry = 0;
        for (unsigned int j = 0; j < maxLen; ++j) {
            unsigned short tmp; // (2^8 - 1)^2 + 2^8 < 2^16;
            tmp = (static_cast<unsigned short>(secondNum->_digits[i]) * static_cast<unsigned short>(firstNum->_digits[j])) + carry;
            temp._digits[j] = tmp % base;
            carry = tmp / base; // (2^16-1) / (2^8 - 1) <= 2^16-1
            if (j == maxLen - 1 && carry != 0 ) {
                temp._digits[maxLen] = carry; // 12*9  = 108
                temp._length++;
            }
        }
        result+=temp;
        if (i != 0)
            result._digits = shift(result._digits,&result._length);
    }
    *this = result;
    return *this;

}

StrNumPositive StrNumPositive::pow(const StrNumPositive& num, unsigned int deg){
    if (deg == 0)
        return StrNumPositive(1);
    if (deg == 1)
        return num;
    StrNumPositive ans(1);
    for (unsigned int i = 0; i < deg; ++i) {
        ans*=num;
    }
    return ans;
}

bool StrNumPositive::operator==(const StrNumPositive& num) const{
    if(this->_length != num._length)
        return false;

    for (unsigned int i = 0; i < num._length; ++i) {
        if(this->_digits[i] != num._digits[i])
            return false;
    }

    return true;

}

StrNumPositive::~StrNumPositive() {
    delete[] _digits;
}