#pragma once
#include "../WAVFormat.h"
#include "../Exceptions.h"
#include "Reader.h"

class WAVReader : public Reader{
public:
    WAVReader(std::string_view path) : Reader(path){_wavHeader.listExist = false;};
    void readHeader();
    void printHeader();
    const Header* getHeader();
    ~WAVReader() override;
private:
    Header _wavHeader;
};