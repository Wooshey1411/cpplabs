#include <iostream>

class StrNumPositive{
    friend StrNumPositive operator+(const StrNumPositive&, const StrNumPositive&);
    friend StrNumPositive operator*(const StrNumPositive&, const StrNumPositive&);
private:
    unsigned char* _digits;
    unsigned int _length;
public:
    unsigned int length() {return _length;}
    unsigned char digits(unsigned int position) {return _digits[position];}

    static StrNumPositive pow(const StrNumPositive& num, unsigned int deg);

    StrNumPositive();
    StrNumPositive(unsigned long);
    StrNumPositive(const std::string&);
    StrNumPositive(const StrNumPositive&);

    StrNumPositive& operator=(const StrNumPositive&);
    StrNumPositive& operator+=(const StrNumPositive&);
    StrNumPositive& operator*=(const StrNumPositive&);

    bool operator==(const StrNumPositive&) const;

    ~StrNumPositive();
};