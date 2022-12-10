#pragma once
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

class Processor {
public:
    Processor() = default;
    void convert(std::string_view in, std::string_view out, std::vector<std::variant<std::string,unsigned int>> &args);
    ~Processor() = default;
};

