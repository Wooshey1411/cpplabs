#pragma once
#include <iostream>
#include <fstream>
#include "../BufferPipeline.h"
class Reader {
public:
    Reader(std::string_view);
    bool read(BufferPipeline&,unsigned int);
    bool readFullBuffer(BufferPipeline&);
    virtual ~Reader() = default;
protected:
    std::fstream _in;
    std::string _path;
};