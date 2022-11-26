#pragma once
#include "header.h"
#include <iostream>


class WAVWriter {
public:
    void writeHeader(const Header*);
    WAVWriter(std::string_view);
    ~WAVWriter() = default;

private:
    std::string _path;
    FILE* _writer;
};
