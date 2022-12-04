#include <iostream>
#include "Processor.h"
#include <vector>
#include <cstring>
#include "ConfigParser.h"
#include "Exceptions.h"
#include "MainUtils.h"

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
        std::vector<std::shared_ptr<Params>> params;
        std::vector<std::string> config;
        ConfigParser configParser(files[0]);
        try {
            configParser.parse(config);
            getParamsAndConverters(files,config,params,converters);
        } catch(BadConfigException& e) {
            std::cerr << e.what() << "\n";
            return BAD_CONFIG;
        }
        catch(NoConfigException& e){
            std::cerr << e.what() << "\n";
            return NO_CONFIG_FILE;
        }

        Processor processor;
        bool permutator = false;

        unsigned int pos = 0;
        bool permutated = false;
        try {
            for (auto it = converters.begin(); it != converters.end(); it++) {
                if (pos == converters.size() - 1) {
                    if (!permutated) {
                        processor.convert(files[2], files[1], *it, std::move(params[pos]));
                    } else {
                        processor.convert(permutator ? "tmp2" : "tmp1", files[1], *it, std::move(params[pos]));
                    }
                    break;
                } else {

                    if (!permutated) {
                        processor.convert(files[2], "tmp1", *it, std::move(params[pos]));
                        permutated = true;
                    } else {
                        processor.convert(permutator ? "tmp2" : "tmp1", permutator ? "tmp1" : "tmp2", *it, std::move(params[pos]));
                        permutator = !permutator;
                    }
                }
                pos++;
            }
        }
        catch (NoFileException& e){
            std::cerr << e.what();
            return NO_INPUT_FILE;
        }
        catch (BadHeaderException& e){
            std::cerr << e.what();
            return BAD_HEADER;
        }

        if(converters.size() == 2){
            remove("tmp1");
        }

        if(converters.size() >=3){
            remove("tmp1");
            remove("tmp2");
        }
    }
    return 0;
}
