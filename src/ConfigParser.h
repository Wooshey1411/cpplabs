#pragma once
#include <vector>
#include <string>
#include <variant>

class ConfigParser {

public:
    ConfigParser(const std::string&);
    void parse(std::vector<std::variant<std::string,unsigned int>> &args);
private:
    std::string _path;
};