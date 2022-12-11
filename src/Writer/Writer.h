#pragma once
#include <fstream>
#include <string>
#include "../BufferPipeline.h"

class Writer {
public:
    Writer(std::string_view);
    void write(BufferPipeline&, unsigned int);
    virtual ~Writer() = default;
protected:
    std::string _path;
    std::fstream _out;
};
