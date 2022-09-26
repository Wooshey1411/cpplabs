#pragma once
#include <iostream>

class BigInt{
    friend BigInt operator+(const BigInt&, const BigInt&);
    friend BigInt operator-(const BigInt&, const BigInt&);
    friend BigInt operator*(const BigInt&, const BigInt&);


private:
    unsigned long* _numberDigits;
    unsigned int _countOfDigits;
    bool _isPositive;
public:
    unsigned long* numberDigits() {return _numberDigits;} // delete in future
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
    // unary operations
    BigInt operator+() const;  // unary +
    BigInt operator-() const;  // unary -

    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator--();
    BigInt operator--(int);


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

    explicit operator int() const;

    ~BigInt();
};

