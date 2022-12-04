#pragma once
#include "Converters.h"
#include <memory>
#include <iostream>
#include <map>
#include <vector>
class ConvertersFactory {
public:
    ConvertersFactory();
    std::unique_ptr<Converter> createConverter(std::string_view);
    void printDescriptions();
    ~ConvertersFactory() = default;
private:
    std::map<std::string,std::unique_ptr<Converter> (*)()> _converters;
};
