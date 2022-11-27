#include <iostream>

class Processor {
public:
    Processor() = default;
    void convert(std::string_view in, std::string_view out, std::string_view name, void* params);
    ~Processor() = default;
};

