#include "ConfigParser.h"
#include <fstream>
#include "Params/Params.h"
#include "Exceptions.h"
#include <variant>


ConfigParser::ConfigParser(const std::string& path):_path(path) {}

void ConfigParser::parse(std::vector<std::string> &config) {
    std::ifstream in;
    in.open(_path);
    if(!in.is_open()){
        throw NoConfigException();
    }

    while(!in.eof()) {
        std::string str;
        in >> str;
        if(str[0] == '#'){
            std::getline(in,str);
            continue;
        }
        config.push_back(str);
    }
}