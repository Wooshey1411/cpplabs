#pragma once
#include "../Writer/Writer.h"
#include "../Reader/Reader.h"
#include <memory>
#include <variant>
#include <vector>

class Converter{
public:
    virtual unsigned int getCountOfParams() = 0;
    virtual void convert(std::vector<std::variant<std::string,unsigned int>> &params,std::vector<std::unique_ptr<Reader>> &streams,Writer out, uint32_t frequency) = 0;
    virtual void printDescription() = 0;
    virtual ~Converter() = default;
};