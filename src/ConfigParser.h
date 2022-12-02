#pragma once
#include <vector>
#include <string>

class ConfigParser {

public:
    ConfigParser(std::string);
    void parse(std::vector<std::string> &config);
    ~ConfigParser() = default;
private:
    std::string _path;
};