//
// Created by Admin on 13.09.2022.
//

#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>

class BigInt{
private:
    unsigned long* _numberDigits;
    unsigned int _countOfDigits;
    bool _isPositive;
public:
    unsigned long* numberDigits() {return _numberDigits;}
    [[nodiscard]] unsigned int countOfDigits() const {return _countOfDigits;}
    [[nodiscard]] bool isPositive() const {return _isPositive;}
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

    BigInt& operator+=(const BigInt&);
    BigInt& operator-=(const BigInt&);
    BigInt& operator*=(const BigInt&);

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
