#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "header.h"

class WAVReader{
public:
    WAVReader(std::string_view);
    void readHeader();
    void printHeader();
    const Header* getHeader();
    ~WAVReader();
private:
    std::string _path;
    FILE* _reader;
    Header _wavHeader;
};