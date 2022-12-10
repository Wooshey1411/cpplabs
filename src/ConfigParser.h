#pragma once
#include <vector>
#include <string>

class ConfigParser {

public:
    ConfigParser(const std::string&);
    void parse(std::vector<std::string> &config);
private:
    std::string _path;
};