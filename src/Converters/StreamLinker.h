#pragma once
#include "../BufferPipeline.h"
#include <iostream>
#include "../WAVReader.h"

class StreamLinker {
public:
    StreamLinker(const std::string&);
    void fillBuffer(BufferPipeline*);
    ~StreamLinker() = default;
private:
    WAVReader* _reader;
};
