#include "BigInt.h"

#include <memory>
const unsigned short basicBase = 256;
const unsigned short decimalBase = 10;
const unsigned char bytesInInt = sizeof(long);
BigInt abs(const BigInt& num){
    if (num.isPositive())
        return num;
    else return -num;
}

unsigned char* resize(unsigned char* a, unsigned int *size, unsigned int newSize){
    auto* t = new unsigned char[newSize+1];
    std::copy(a, a + newSize, t);
    (*size) = newSize;
    delete[] a;
    return t;
}

unsigned char* shift(const unsigned  char* a, unsigned int *size){
    auto* t = new unsigned char [*size + 1];
    for (unsigned long i = 0; i < *size; ++i)
        t[i+1] = a[i];
    t[0] = 0;
    (*size)++;
    delete[] a;
    return  t;
}

unsigned int pow(unsigned int a, unsigned int deg){
    if (deg == 0)
        return 1;
    if (deg == 1)
        return a;
    unsigned int res = 1;
    for (unsigned int i = 0; i < deg; ++i) {
        res*=a;
    }
    return res;
}

BigInt pow(BigInt& num,unsigned int deg, unsigned short base){
    if (deg == 0)
        return BigInt(1,base);
    if (deg == 1)
        return num;
    BigInt ans(1,base);
    for (unsigned int i = 0; i < deg; ++i) {
        ans*=num;
    }
    return ans;
}

// mode 'i' integer part (62/3 = 20), mode 'r' - rest part (62%3 = 2)
BigInt BigInt::div (const BigInt& num, char mode){

    if(num == BigInt(0))
        throw std::runtime_error("Division by zero");

    if (mode == 'r' && abs(*this)==abs(num)) // 62 % 62 = 0
        return BigInt();
    if(mode == 'r' && abs(*this) < abs(num)) // 53 % 62 = 53
        return *this;
    if (mode == 'i' && abs(*this) < abs(num)) // 5 / 52 = 0
        return BigInt();

    BigInt ans;
    ans._numberDigits = resize(ans._numberDigits, &ans._countOfDigits, 0);
    BigInt temp;
    BigInt absNum = abs(num);
    bool takeDigits = false;

    for (unsigned int i = 0; i < this->_countOfDigits; ++i) {
        if(temp != BigInt())
            temp._numberDigits = shift(temp._numberDigits,&temp._countOfDigits);
        temp._numberDigits[0] = this->_numberDigits[this->_countOfDigits - 1 - i];
        if (temp >= absNum){
            takeDigits = true;
            unsigned long digit = 0;
            while (temp >= absNum){
                digit++;
                temp-=absNum;
            }

            ans._numberDigits = shift(ans._numberDigits, &ans._countOfDigits);
            ans._numberDigits[0] = digit;

        } else if (takeDigits){
            ans._numberDigits = shift(ans._numberDigits, &ans._countOfDigits);
            ans._numberDigits[0] = 0;
        }
    }

    ans._isPositive = !(this->_isPositive ^ num._isPositive);

    if (mode == 'i')
        return ans;

    if (mode == 'r'){
        if (this->_isPositive)
            return temp;
        else
            return absNum - temp;
    }

    return BigInt();
}

BigInt::BigInt()
    :_numberDigits(new unsigned char[1]), _countOfDigits(1),_isPositive(true),base(basicBase){
    _numberDigits[0] = 0;
}

BigInt::BigInt(long num, unsigned short b)
:_numberDigits(new unsigned char[bytesInInt]),base(Readonly(b)){
    if(num < 0)
        _isPositive = false;
    else
        _isPositive = true;

    unsigned long temp = abs(num);
    unsigned int len = 0;
    while(true){
        _numberDigits[len] = temp % base.getInstance();
        temp/=base.getInstance();
        len++;
        if(temp == 0)
            break;
    }
    _countOfDigits = bytesInInt;
    _numberDigits = resize(_numberDigits, &_countOfDigits,len);
}

BigInt::BigInt(long num)
:_numberDigits(new unsigned char[bytesInInt]),base(basicBase){
    if(num < 0)
        _isPositive = false;
    else
        _isPositive = true;

    unsigned long temp = abs(num);
    unsigned int len = 0;
    while(true){
        _numberDigits[len] = temp % base.getInstance();
        temp/=base.getInstance();
        len++;
        if(temp == 0)
            break;
    }
    _countOfDigits = bytesInInt;
    _numberDigits = resize(_numberDigits, &_countOfDigits,len);
}

BigInt::BigInt(const std::string& num)
:base(basicBase){
    size_t length = num.size();


    if((!isdigit(num[0]) && num[0] != '-') || (num[0] == '-' && length == 1)) // check input data on correct of value
        throw std::runtime_error("Error - wrong symbol in string number");

    for (size_t i = 0; i < length; ++i) {
        if(!std::isdigit(num[i]) && i != 0)
            throw std::runtime_error("Error - wrong symbol in string number");
    }

    _numberDigits = new unsigned char[0];
    auto newNumber = std::make_unique<unsigned char[]>(num.size());
    int currDigitPos = 0;
    _countOfDigits = 0;
    unsigned char shifter = 0;

    if (num[0] == '-') { // first char can be a symbol '-' if it right we need to shift string on 1 position left. It's doing by shifter
        shifter = 1;
        _isPositive = false;
    }
    else
        _isPositive = true;

    for (size_t i = 0+shifter; i < length; ++i) {
        newNumber[i - shifter] = (num[i] - '0');
    }

    length-=shifter;

    while (true){ // building number in base 2^32 by division into a column
        unsigned short temp = 0;
        int countOfNewDigits = 0;
        bool takeDigits = false;

        for (size_t i = 0; i < length; ++i){
            temp = 10*temp + (newNumber[i]);
            if (temp >= base.getInstance()){
                takeDigits = true;
                newNumber[countOfNewDigits] = static_cast<unsigned char>(temp/base.getInstance()); // newNumber values [0:9]
                temp%=base.getInstance();
                countOfNewDigits++;
            }
            else if (takeDigits){
                newNumber[countOfNewDigits] = 0;
                countOfNewDigits++;
            }
        }
        _numberDigits = resize(_numberDigits,&_countOfDigits,_countOfDigits + 1);
        _numberDigits[currDigitPos] = static_cast<unsigned char>(temp); // temp after temp=%base cannot be more than 2^8 - 1. I can take in unsigned long a number like temp
        currDigitPos++;
        length = countOfNewDigits;
        if(countOfNewDigits == 0)
            break;
    }

    if(_countOfDigits == 1 && _numberDigits[0] == 0)
        _isPositive = true;

}

BigInt::BigInt(const BigInt& num)
    :_countOfDigits(num._countOfDigits), _isPositive(num._isPositive),base(num.base.getInstance()){
    this->_numberDigits = new unsigned char[num._countOfDigits];
    for (unsigned int i = 0; i < num._countOfDigits; ++i) {
        this->_numberDigits[i] = num._numberDigits[i];
    }
}

BigInt::BigInt(BigInt&& num) noexcept
    :_numberDigits(nullptr),_countOfDigits(0),_isPositive(false),base(0){
    _numberDigits = num._numberDigits;
    _countOfDigits = num._countOfDigits;
    _isPositive = num._isPositive;
    base = Readonly(num.base.getInstance());

    num._numberDigits = nullptr;
    num._countOfDigits = 0;
    num._isPositive = false;
}

BigInt& BigInt::operator=(const BigInt& num){
    if (this != &num) {
        this->_countOfDigits = num._countOfDigits;
        this->_isPositive = num._isPositive;
        delete[] this->_numberDigits;
        this->_numberDigits = new unsigned char[num._countOfDigits];
        for (unsigned int i = 0; i < num._countOfDigits; ++i) {
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
    if(*this != BigInt())
         x._isPositive = !x._isPositive;
    return x;
}
BigInt BigInt::operator~() const{
    BigInt x(*this);
    x._isPositive = !x._isPositive;
    for (unsigned int i = 0; i < x._countOfDigits; ++i) {
        x._numberDigits[i] = ~this->_numberDigits[i];
    }
    return x;
}

BigInt& BigInt::operator++(){
    *this += BigInt(1);
    return *this;
}
BigInt BigInt::operator++(int) {
    BigInt x(*this);
    *this += BigInt(1);
    return x;
}
BigInt& BigInt::operator--(){
    *this -= BigInt(1);
    return *this;
}
BigInt BigInt::operator--(int) {
    BigInt x(*this);
    *this -= BigInt(1);
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
        this->_numberDigits = resize(this->_numberDigits, &this->_countOfDigits, num._countOfDigits);
        maxNum = &num;
    }

    unsigned char carry = 0;
    for (unsigned int i = 0; i < maxLen; ++i) {
        unsigned short tmp;

        if(i < minLen)
            tmp = carry + static_cast<unsigned short>(this->_numberDigits[i]) + static_cast<unsigned short>(num._numberDigits[i]);
        else
            tmp = static_cast<unsigned short>(maxNum->_numberDigits[i]) + carry;

        this->_numberDigits[i] = tmp % base.getInstance(); // tmp % base <= 2^8 - 1
        carry = tmp / base.getInstance(); // carry is [0:1];

        if(i == maxLen - 1 && carry == 1) // 999 + 1 = 1000
        {
            this->_numberDigits = resize(this->_numberDigits, &this->_countOfDigits, this->_countOfDigits + 1);
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
    }else{ // |a| < |b|. If b - a => sign + (*this have sign -). If a - b => sign + (*this have sign +) -> we need resign *this in both cases;
        minLen = this->_countOfDigits;
        maxLen = num._countOfDigits;
        firstNum = &num;
        secondNum = this;
        this->_isPositive = !this->_isPositive;
    }
    this->_numberDigits = resize(this->_numberDigits, &this->_countOfDigits, maxLen);

    unsigned char debt = 0;
    for (unsigned int i = 0; i < maxLen; ++i) {
        short tmp;
        if(i<minLen){
            tmp = static_cast<short>(firstNum->_numberDigits[i]) - static_cast<short>(secondNum->_numberDigits[i]) -debt;

            if (tmp < 0)
            {
                tmp +=base.getInstance();
                debt = 1;
            }else
                debt = 0;

            this->_numberDigits[i] = static_cast<unsigned char>(tmp); // tmp <= base - 1;
        }else{
            if(firstNum->_numberDigits[i] == 0 && debt == 1) // 10000 - 1 there for second digit debt = 1 and digit = 0 => digit became a 9 (10 - 1);
                this->_numberDigits[i] = base.getInstance() - 1;
            else if(firstNum->_numberDigits[i] != 0 && debt == 1) { // 19990 - 1 there for second digit debt = 1 and digit = 9 => digit = 8 and debt = 0
                this->_numberDigits[i] = firstNum->_numberDigits[i] - 1;
                debt = 0;
            } else
                this->_numberDigits[i] = firstNum->_numberDigits[i]; // 19991 - 1 there debt = 0 for 9
        }
    }

    unsigned int liveDigit = 0; // 67700 - 67695 = 5. only 1 digit is "live" his position is 0
    for (unsigned int i = 0; i < maxLen; ++i) {
        if (this->_numberDigits[i] != 0)
            liveDigit = i;
    }

    this->_numberDigits = resize(this->_numberDigits, &this->_countOfDigits, liveDigit + 1);
    if(this->_countOfDigits == 1 && this->_numberDigits[0] == 0)
        this->_isPositive = true;
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
    temp._numberDigits = resize(temp._numberDigits,&temp._countOfDigits, std::max(this->_countOfDigits,num._countOfDigits) + 1); // 999*9=8991 - 4 digits nax
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
    unsigned char carry; // carry <= sizeof(unsigned char) - 1;
    for (long long i = minLen - 1; i >= 0 ; --i) {
        temp._countOfDigits = maxLen;
        carry = 0;
        for (unsigned int j = 0; j < maxLen; ++j) {
            unsigned short tmp; // (2^8- 1)^2 + 2^8 < 2^16;
            tmp = (static_cast<unsigned short>(secondNum->_numberDigits[i]) * static_cast<unsigned short>(firstNum->_numberDigits[j])) + carry;
            temp._numberDigits[j] = static_cast<unsigned short>(tmp % base.getInstance()); // base = 256 -> tmp % base <=255
            carry = tmp / base.getInstance(); // (2^16-1) / (2^8) <= 2^8-1
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
BigInt& BigInt::operator/=(const BigInt& num){
    *this = div(num,'i');
    return *this;
}
BigInt& BigInt::operator%=(const BigInt& num){
    *this = div(num,'r');
    return *this;
}
BigInt& BigInt::operator&=(const BigInt& num){
    unsigned int minLen = std::min(this->_countOfDigits,num._countOfDigits);
        for (unsigned int i = 0; i < minLen; ++i) {
            this->_numberDigits[i] = this->_numberDigits[i] & num._numberDigits[i];
        }

        this->_isPositive = this->_isPositive || num._isPositive;
        this->_numberDigits = resize(this->_numberDigits, &this->_countOfDigits, minLen);
        return *this;
}
BigInt& BigInt::operator|=(const BigInt& num) {
    unsigned int minLen = std::min(this->_countOfDigits,num._countOfDigits);
    for (unsigned int i = 0; i < minLen; ++i) {
        this->_numberDigits[i] = this->_numberDigits[i] | num._numberDigits[i];
    }

    this->_isPositive = this->_isPositive || num._isPositive;

    if(this->_countOfDigits >= num._countOfDigits)
        return *this;

    this->_numberDigits = resize(this->_numberDigits, &this->_countOfDigits, num._countOfDigits);
    for (unsigned int i = minLen; i < num._countOfDigits; ++i) {
        this->_numberDigits[i] = num._numberDigits[i];
    }
    return *this;
}




BigInt operator+(const BigInt& num1, const BigInt& num2){
    BigInt x(num1);
    return x+=num2;
}
BigInt operator-(const BigInt& num1, const BigInt& num2){
    BigInt x(num1);
    return x-=num2;
}
BigInt operator*(const BigInt& num1, const BigInt& num2){
    BigInt x(num1);
    return x*=num2;
}
BigInt operator/(const BigInt& num1, const BigInt& num2){
    BigInt x(num1);
    return x/=num2;
}
BigInt operator%(const BigInt& num1, const BigInt& num2){
    BigInt x(num1);
    return x%=num2;
}
BigInt operator&(const BigInt& num1, const BigInt& num2){
    BigInt x(num1);
    x&=num2;
    return x;
}
BigInt operator|(const BigInt& num1, const BigInt& num2){
    BigInt x(num1);
    x|=num2;
    return x;
}

bool BigInt::operator==(const BigInt& num) const
{
    if((this->_isPositive && !num._isPositive) || (!this->_isPositive && num._isPositive) || (this->_countOfDigits != num._countOfDigits)) // different signs
        return false;
    for (unsigned int i = 0; i < this->_countOfDigits; ++i) {
        if(this->_numberDigits[i] != num._numberDigits[i])
            return false;
    }
    return true;
}
bool BigInt::operator!=(const BigInt& num) const{
    return !(*this == num);
}
bool BigInt::operator<(const BigInt& num) const{
    if ((this->_isPositive && !num._isPositive) || this->_countOfDigits > num._countOfDigits || operator==(num))
        return false;
    if((!this->_isPositive && num._isPositive) || this->_countOfDigits < num._countOfDigits)
        return true;

    for (unsigned int i = 0; i < num._countOfDigits; ++i) { // both numbers have equal count of digits and sign

        if ((this->_numberDigits[num._countOfDigits-1-i] > num._numberDigits[num._countOfDigits-1-i] && num._isPositive) ||
                (this->_numberDigits[num._countOfDigits-1-i] < num._numberDigits[num._countOfDigits-1-i] && !num._isPositive)) // -111 > -222 but 111 < 222
            return false;
        if (this->_numberDigits[num._countOfDigits-i-1] != num._numberDigits[num._countOfDigits-i-1])
            return true;
    }

    return true;
}
bool BigInt::operator>(const BigInt& num) const{ // > ~ not < and not == because !< = >=
    if(!(*this < num) && *this != num)
        return true;
    return false;
}
bool BigInt::operator<=(const BigInt& num) const{
    if (!(*this > num))
        return true;
    return false;
}
bool BigInt::operator>=(const BigInt& num) const
{
    if(!(*this < num))
        return true;
    return false;
}

BigInt::operator int() const{
    unsigned char pos = 0;
    int res = 0;
    while(true){
        res+= this->_numberDigits[pos]*pow(base.getInstance(),pos);
        pos++;
        if(pos == 4 || pos == this->_countOfDigits)
            break;
    }
    if(!this->_isPositive)
        return -res;
    return res;
}
BigInt::operator std::string() const{
    BigInt str(0,decimalBase);
    BigInt baseStr(this->base.getInstance(),decimalBase);

    for (unsigned int i = 0; i < this->_countOfDigits; ++i) {
        str += (BigInt(this->_numberDigits[i],decimalBase) * pow(baseStr,i,baseStr.base.getInstance()));
    }
    std::string ans;
    unsigned char signShifter = 0;
    if (!this->_isPositive)
        signShifter = 1;
    ans.resize(str._countOfDigits + signShifter);
    if(signShifter == 1)
        ans[0] = '-';
    for (unsigned int i = signShifter; i < str._countOfDigits+signShifter; ++i) {
        ans[i]= str._numberDigits[str._countOfDigits-i-1+signShifter]+'0';
    }

    return ans;
}
size_t BigInt::size() const{
    return sizeof(BigInt) + this->_countOfDigits;
}
std::ostream& operator<<(std::ostream& s, const BigInt& i){
    s << std::string(i);
    return s;
}
std::istream& operator>>(std::istream& s, BigInt& i){
    int currChar;
    std::string conStr(" ");
    std::string stringToBigInt("");
    bool isFirst = true;
    while(true) {
        currChar = getchar();
        if((std::isspace(currChar) || !std::isdigit(currChar)) && !isFirst)
            break;
        if (isFirst && (currChar != '-' && !std::isdigit(currChar)))
            break;
        conStr[0] = currChar;
        isFirst = false;
        stringToBigInt+=conStr;
    }

    if(stringToBigInt != "")
         i = BigInt(stringToBigInt);
    else
        i = BigInt();
    return s;
}


BigInt::~BigInt()
{
    delete[] _numberDigits;
}
