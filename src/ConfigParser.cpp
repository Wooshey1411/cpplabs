#include "ConfigParser.h"
#include <fstream>
#include "Exceptions.h"

unsigned int getValue(const std::string& str, bool skip){
    unsigned int value = 0;
    for (unsigned int i = 0; i < str.size(); i++) {
        if (!(skip && i == 0)){
            if (!std::isdigit(str[i])) {
                throw BadConfigException();
            }
            value = value * 10 + (str[i] - '0');
        }
    }
    return value;
}

ConfigParser::ConfigParser(const std::string& path):_path(path) {}

void ConfigParser::parse(std::vector<std::variant<std::string,unsigned int>> &args) {
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
        std::variant<std::string,unsigned int> a;
        if(str[0] == '$'){
            a = getValue(str, true);
        } else if(isdigit(str[0])){
             a = getValue(str, false);
        } else{
             a = str;
        }
        args.push_back(a);
    }
}