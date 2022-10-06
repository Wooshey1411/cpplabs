#pragma once
#include <iostream>
#include "Readonly.h"

class BigInt{
    friend BigInt pow(BigInt& num,unsigned int deg, unsigned short base);
private:
    BigInt(long num,unsigned short base); // constructor for internal methods.
    unsigned char* _numberDigits;
    unsigned int _countOfDigits;
    bool _isPositive;
    BigInt div (const BigInt& num, char mode);
    Readonly base;
public:
    unsigned char numberDigits(unsigned int position) {return _numberDigits[position];} // delete in future
    [[nodiscard]] unsigned int countOfDigits() const {return _countOfDigits;} // delete in future
    [[nodiscard]] bool isPositive() const {return _isPositive;}
    // constructors
    BigInt();
    explicit BigInt(long);
    explicit BigInt(const std::string&);
    BigInt(const BigInt&);
    BigInt(BigInt&&) noexcept;

    BigInt& operator=(const BigInt&);
    BigInt& operator=(BigInt&&) noexcept;

    BigInt operator+() const;  // unary +
    BigInt operator-() const;  // unary -
    BigInt operator~() const;

    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator--();
    BigInt operator--(int);


    BigInt& operator+=(const BigInt&);
    BigInt& operator-=(const BigInt&);
    BigInt& operator*=(const BigInt&);
    BigInt& operator/=(const BigInt&);
    BigInt& operator%=(const BigInt&);
    BigInt& operator&=(const BigInt&);
    BigInt& operator|=(const BigInt&);


    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator<(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>=(const BigInt&) const;

    explicit operator int() const;
    operator std::string() const;
    size_t size() const;


    ~BigInt();
};

BigInt operator+(const BigInt&, const BigInt&);
BigInt operator-(const BigInt&, const BigInt&);
BigInt operator*(const BigInt&, const BigInt&);
BigInt operator/(const BigInt&, const BigInt&);
BigInt operator%(const BigInt&, const BigInt&);
BigInt operator&(const BigInt&, const BigInt&);
BigInt operator|(const BigInt&, const BigInt&);
std::ostream& operator<<(std::ostream& s, const BigInt& i);
std::istream& operator>>(std::istream& s, BigInt& i);