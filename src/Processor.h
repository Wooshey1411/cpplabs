#pragma once
#include <iostream>
#include <memory>
#include "Params/Params.h"
class Processor {
public:
    Processor() = default;
    void convert(std::string_view in, std::string_view out, std::string_view name, const std::shared_ptr<Params>& params);
    ~Processor() = default;
};

