#pragma once
#include "header.h"
#include "BufferPipeline.h"
#include <iostream>


class WAVWriter {
public:
    WAVWriter(std::string_view);
    void writeHeader(const Header*);
    void writeSecond(BufferPipeline*);
    ~WAVWriter() = default;

private:
    std::string _path;
    FILE* _writer;
};
