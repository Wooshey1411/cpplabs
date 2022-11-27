#include <iostream>
#include <fstream>
#include <string>
#include "Processor.h"
#include "Params.h"
#include <vector>
#include <cstring>
#include "ConfigParser.h"

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

int main(int argc, char* argv[]) {
    if(!strcmp(argv[1],"-h") && argc == 2){
        printHelp();
        return 0;
    }

    if(argc > 2 && !strcmp(argv[1],"-c")){
        std::vector<std::string> files;
        for (int i = 2; i < argc; ++i) {
            files.push_back(std::string(argv[i]));
        }
        std::vector<std::string> converters;
        std::vector<void*> params;
        ConfigParser configParser;
        configParser.parse(files,params,converters);

        Processor processor;
        bool permutator = false;

        unsigned int pos = 0;
        bool permutated = false;
        for(auto it = converters.begin(); it != converters.end(); it++)
        {
            if(pos == converters.size() - 1){
                if(!permutated){
                    processor.convert(files[2],files[1],*it,params[pos]);
                } else {
                    processor.convert(permutator ? "tmp2" : "tmp1", files[1], *it, params[pos]);
                }
                break;
            } else{

                if(!permutated){
                    processor.convert(files[2],"tmp1",*it,params[pos]);
                    permutated = true;
                } else{
                    processor.convert(permutator ? "tmp2" : "tmp1", permutator ? "tmp1" : "tmp2",*it,params[pos]);
                    permutator = !permutator;
                }
            }
            pos++;
        }
        if(converters.size() == 2){
            remove("tmp1");
        }
        if(converters.size() >=3){
            remove("tmp1");
            remove("tmp2");
        }

        pos = 0;


        for (auto & param : params) {
            if(converters[pos] == "mute"
            || converters[pos] == "bassBoosted"){
                delete static_cast<IFParams*>(param);
            } else if(converters[pos] == "mix"){
                delete static_cast<PIParams*>(param);
            } else if(converters[pos] == "distortion"){
                delete static_cast<IFCParams*>(param);
            }
            pos++;
        }


    }

    return 0;
}
