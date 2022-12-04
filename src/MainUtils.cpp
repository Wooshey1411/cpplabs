#include "MainUtils.h"
#include "Exceptions.h"
#include "Converters/ConvertersFactory.h"
#include <any>
#include <iomanip>

void printHelp(){
    for (int i = 0; i < 100; ++i) {
        std::cout << '-';
    }
    std::cout << "\n";
    std::cout << "NAME\n";
    std::cout << std::setw(10) << std::cout.fill() << "SoundProcessor - utility for transformations on wav files\n";

    std::cout << "SYNOPSIS\n";
    std::cout << std::setw(10) << std::cout.fill() << "SoundProcessor [-c config.txt output.wav input1.wav [input2.wav ...]]\n";

    std::cout << "DESCRIPTION\n";
    std::cout << std::setw(10) << std::cout.fill() << "Information of convertion on input1.wav stores in config.txt\n";
    std::cout << std::setw(10) << std::cout.fill() << "Converted file write in output.wav\n";
    std::cout << std::setw(10) << std::cout.fill() << "input2.wav ... need for some converters\n";
    std::cout << std::setw(10) << std::cout.fill() << "Every string in config.txt includes name of conversion and params\n";
    std::cout << std::setw(10) << std::cout.fill() << "In config.txt allows comment-string. They start with symbol '#'\n";

    std::cout << "CONVERTERS\n";
    ConvertersFactory convertersFactory;
    convertersFactory.printDescriptions();
    for (int i = 0; i < 100; ++i) {
        std::cout << '-';
    }
    std::cout << "\n";
}


enum Codes{
    IF = 0,
    PIP,
    IFC
};

unsigned int getCode(const std::string& name){
    if(name == "mute"){
        return Codes::IF;
    }
    if(name == "bassBoosted"){
        return Codes::IF;
    }
    if(name == "mix"){
        return Codes::PIP;
    }
    if(name == "distortion"){
        return Codes::IFC;
    }
    throw BadConfigException();
};


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

void getParamsAndConverters(std::vector<std::string> files, std::vector<std::string> config, std::vector<std::shared_ptr<Params>> &params, std::vector<std::string> &converters){
    auto iterator = config.begin();
    while(true){
        if(iterator == config.end()){
            break;
        }

        std::string name = *iterator;
        iterator++;
        unsigned int code = getCode(name);
        converters.push_back(name);
        switch (code) {
            case Codes::IF: {
                auto param = std::make_shared<IFParamsC>();
                for (int i = 1; i <= param->countOfParams(); ++i) {
                    if(iterator == config.end()){
                        throw BadConfigException();
                    }
                    param->setParams(i, std::make_any<unsigned int>(getValue(*iterator, false)));
                    iterator++;
                }
                params.push_back(std::move(param));
                break;
            }
            case Codes::IFC:{
                auto param = std::make_shared<IFCParamsC>();
                for (int i = 1; i <= param->countOfParams(); ++i) {
                    if(iterator == config.end()){
                        throw BadConfigException();
                    }
                    param->setParams(i, std::make_any<unsigned int>(getValue(*iterator, false)));
                    iterator++;
                }
                params.push_back(std::move(param));
                break;
            }
            case::Codes::PIP:{
                auto param = std::make_shared<PIPParamsC>();
                if(iterator == config.end()){
                    throw BadConfigException();
                }
                if ((*iterator)[0] != '$'){
                    throw BadConfigException();
                }
                unsigned int pos = getValue(*iterator,true);

                if(pos+2 >= files.size()){
                    throw BadConfigException("Invalid #NUMBER");
                }

                param->setParams(1,std::make_any<std::string>(files[2+pos]));
                iterator++;
                if(iterator == config.end()){
                    throw BadConfigException();
                }
                param->setParams(2, std::make_any<unsigned int>(getValue(*iterator, false)));
                iterator++;
                params.push_back(std::move(param));
                break;
            }
            default:
                throw BadConfigException();
        }

    }
}