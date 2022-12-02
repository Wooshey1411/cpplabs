#pragma once
#include "Converters.h"
#include <memory>
#include <iostream>
#include <map>
class ConvertersFactory {
public:
    ConvertersFactory();
    std::unique_ptr<Converter> createConverter(std::string_view);
    ~ConvertersFactory() = default;
private:
    std::map<std::string,std::unique_ptr<Converter> (*)()> _converters;
};
