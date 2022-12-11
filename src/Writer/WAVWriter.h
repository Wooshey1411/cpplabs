#pragma once
#include "../WAVheader.h"
#include "../BufferPipeline.h"
#include "Writer.h"

class WAVWriter : public Writer{
public:
    WAVWriter(std::string_view path) : Writer(path){};
    void writeHeader(const Header*);
   // void writeByFrequency(BufferPipeline*);
    ~WAVWriter() override = default;
};
