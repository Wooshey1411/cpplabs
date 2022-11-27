#pragma once
#include <vector>
#include <string>

class ConfigParser {
public:
    void parse(std::vector<std::string> &files, std::vector<void *> &params, std::vector<std::string> &converters);
};