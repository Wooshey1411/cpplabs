#include "MainUtils.h"
#include "Exceptions.h"
#include <any>

void printHelp(){
    std::cout << "NAME\n";
    std::cout << "          SoundProcessor - utility for transformations on wav files\n";

    std::cout << "SYNOPSIS\n";
    std::cout << "          SoundProcessor [-c config.txt output.wav input1.wav [input2.wav ...]]\n";

    std::cout << "DESCRIPTION\n";
    std::cout << "          Information of convertion on input1.wav stores in config.txt\n";
    std::cout << "          Converted file write in output.wav\n";
    std::cout << "          input2.wav ... need for some converters\n";
    std::cout << "          Every string in config.txt includes name of conversion and params\n";
    std::cout << "          In config.txt allows comment-string. They start with symbol '#'\n";

    std::cout << "CONVERTERS\n";
    std::cout << "          mute [INITIAL_SECOND] [FINAL_SECOND]\n";
    std::cout << "               mute audio from INITIAL_SECOND to FINAL_SECOND\n\n";
    std::cout << "          mix [#NUMBER] [INITIAL_SECOND]\n";
    std::cout << "               mix input audio file with another by NUMBER in order of indication in arguments\n";
    std::cout << "               from INITIAL_SECOND to end of file\n\n";
    std::cout << "          bassBoosted [INITIAL_SECOND] [FINAL_SECOND]\n";
    std::cout << "               make bass boosted effect from INITIAL_SECOND to FINAL_SECOND\n\n";
    std::cout << "          distortion [INITIAL_SECOND] [FINAL_SECOND] [COEFFICIENT]\n";
    std::cout << "               make distortion effect from INITIAL_SECOND to FINAL_SECOND\n";
    std::cout << "               COEFFICIENT - level of cutting of wave in percents\n";
}


enum Codes{
    IF = 0,
    PIP,
    IFC
};

unsigned int getCode(std::string name){
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
                unsigned int pos = getValue(*iterator,true);

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