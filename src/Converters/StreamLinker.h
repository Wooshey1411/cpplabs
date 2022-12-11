#pragma once
#include "../BufferPipeline.h"
#include <iostream>
#include "../Reader/WAVReader.h"

class StreamLinker {
public:
    StreamLinker(const std::string&);
   // void fillBuffer(BufferPipeline*);
    ~StreamLinker() = default;
private:
    WAVReader* _reader;
};
