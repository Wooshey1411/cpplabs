#pragma once
#include "../BufferPipeline.h"
#include "../Params/Params.h"
#include <memory>

class Converter{
public:
    virtual void convert( std::shared_ptr<Params> params,BufferPipeline*) = 0;
    virtual void printDescription() = 0;
    virtual ~Converter() = default;
};