#include "ConfigParser.h"
#include <fstream>
#include "Params.h"
#include <iostream>
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

bool ConfigParser::parseString(std::vector<std::string> &files, std::vector<void *> &params, std::vector<std::string> &converters,std::ifstream &in){
    std::string str;
    in >> str;

    if (str[0] == '#'){
        if(in.eof()){
            return false;
        } else{
            return true;
        }
    }

    if(str == "mute" || str == "bassBoosted"){
        std::string strI;
        in >> strI;
        auto* ifParams= new IFParams;
        ifParams->initial = getValue(strI,false);
        in >> strI;
        ifParams->final = getValue(strI, false);
        params.push_back(ifParams);
        _params.push_back(ifParams);
        converters.push_back(str);
    } else if(str == "mix"){
        std::string strI;
        in >> strI;
        auto* piParams= new PIParams;
        unsigned int pos = getValue(strI,true);
        if (pos >= files.size()){
            throw BadConfigException();
        }
        piParams->path = files[2+pos]; // 2 - position of first additional file
        in >> strI;
        piParams->initial = getValue(strI, false);
        params.push_back(piParams);
        _params.push_back(piParams);
        converters.push_back(str);
    } else if(str == "distortion"){
        std::string strI;
        in >> strI;
        auto* ifcParams= new IFCParams;
        ifcParams->initial = getValue(strI,false);
        in >> strI;
        ifcParams->final = getValue(strI, false);
        in >> strI;
        ifcParams->coefficient = getValue(strI,false);
        params.push_back(ifcParams);
        _params.push_back(ifcParams);
        converters.push_back(str);
    } else{
        throw BadConfigException();
    }

    _converters.push_back(str);

    if(in.eof()){
        return false;
    }
    return true;
}

void ConfigParser::parse(std::vector<std::string> &files, std::vector<void *> &params, std::vector<std::string> &converters) {
    std::ifstream in;
    in.open(files[0]);
    if(!in.is_open()){
        throw NoConfigException();
    }

    while(true) {
        bool checker = parseString(files,params,converters,in);
        if (!checker){
            break;
        }
    }
}

ConfigParser::~ConfigParser() {
    unsigned int pos = 0;
    for (auto & param : _params) {
        if(_converters[pos] == "mute"
           || _converters[pos] == "bassBoosted"){
            delete static_cast<IFParams*>(param);
        } else if(_converters[pos] == "mix"){
            delete static_cast<PIParams*>(param);
        } else if(_converters[pos] == "distortion"){
            delete static_cast<IFCParams*>(param);
        }
        pos++;
    }
}