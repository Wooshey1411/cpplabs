#include "ConfigParser.h"
#include <fstream>
#include "Params/Params.h"
#include "Exceptions.h"

ConfigParser::ConfigParser(std::string  path):_path(std::move(path)) {}

void ConfigParser::parse(std::vector<std::string> &config) {
    std::ifstream in;
    in.open(_path);
    if(!in.is_open()){
        throw NoConfigException();
    }

    while(true) {
        if(in.eof()){
            break;
        }
        std::string str;
        in >> str;
        config.push_back(str);
    }
}