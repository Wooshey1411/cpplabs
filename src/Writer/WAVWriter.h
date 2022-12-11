#pragma once
#include "../WAVFormat.h"
#include "../BufferPipeline.h"
#include "Writer.h"

class WAVWriter : public Writer{
public:
    WAVWriter(std::string_view path) : Writer(path){};
    void writeHeader(const Header*);
    ~WAVWriter() override = default;
};
