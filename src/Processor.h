#pragma once
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

class Processor {
public:
    Processor();
    void convert(std::vector<std::string> &files,std::vector<std::variant<std::string,unsigned int>> &args);
    void deleteTempFiles(const std::string& path);

    ~Processor();
private:
    std::string _outPath;
};

