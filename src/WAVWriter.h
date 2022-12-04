#pragma once
#include "WAVheader.h"
#include "BufferPipeline.h"
#include <iostream>
#include <fstream>

class WAVWriter {
public:
    WAVWriter(std::string_view);
    void writeHeader(const Header*);
    void writeSecond(BufferPipeline*);
    ~WAVWriter();

private:
    std::string _path;
    std::fstream _out;
    FILE* _writer;
};
