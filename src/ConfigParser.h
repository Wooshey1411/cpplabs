#pragma once
#include <vector>
#include <string>

class ConfigParser {

public:
    void parse(std::vector<std::string> &files, std::vector<void *> &params, std::vector<std::string> &converters);
    ~ConfigParser();
private:
    std::vector<std::string> _converters;
    std::vector<void *> _params;
    bool parseString(std::vector<std::string> &files, std::vector<void *> &params, std::vector<std::string> &converters,std::ifstream &in);
};