//
// Created by Admin on 13.09.2022.
//

#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>

class BigInt{
public:
    unsigned long* numberDigits;
    int countOfDigits;
    bool isPositive;

    // constructors
    BigInt();
    explicit BigInt(long);
    explicit BigInt(const std::string&);
    BigInt(const BigInt&);
    BigInt(BigInt&&) noexcept;

    BigInt& operator=(const BigInt&);
    BigInt& operator=(BigInt&&) noexcept;
    // unary operations
    BigInt operator+() const;  // unary +
    BigInt operator-() const;  // unary -

    BigInt operator+(const BigInt&);
    BigInt operator-(const BigInt&);

    // bool operations
    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator<(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>=(const BigInt&) const;

    ~BigInt();
};
#endif //BIGINT_H
