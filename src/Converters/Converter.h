#pragma once
#include "../BufferPipeline.h"
#include <memory>
#include <variant>
#include <vector>

class Converter{
public:
    virtual unsigned int getCountOfParams() = 0;
    virtual void convert(std::vector<std::variant<std::string,unsigned int>> &params,BufferPipeline*) = 0;
    virtual void printDescription() = 0;
    virtual ~Converter() = default;
};