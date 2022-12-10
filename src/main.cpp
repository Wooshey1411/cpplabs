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
            files.emplace_back(argv[i]);
        }
        std::vector<std::string> converters;
        std::vector<std::variant<std::string,unsigned int>> args;
        ConfigParser configParser(files[0]);
        try {
            configParser.parse(args);
        } catch(const BadConfigException& e) {
            std::cerr << e.what() << "\n";
            return BAD_CONFIG;
        }catch(const NoConfigException& e){
            std::cerr << e.what() << "\n";
            return NO_CONFIG_FILE;
        }

        Processor processor;
        bool permutator = false;

        bool permutated = false;
        try {
            while(args.begin() != args.end()) {
                    if (args.size() <= 4) {
                        if (!permutated) {
                            processor.convert(files[2], files[1], args);
                        } else {
                            processor.convert(permutator ? "tmp2" : "tmp1", files[1], args);
                        }
                        break;
                    } else {

                        if (!permutated) {
                            processor.convert(files[2], "tmp1", args);
                            permutated = true;
                        } else {
                            processor.convert(permutator ? "tmp2" : "tmp1", permutator ? "tmp1" : "tmp2", args);
                            permutator = !permutator;
                        }
                    }
            }
        }
        catch (const NoFileException& e){
            std::cerr << e.what();
            return NO_INPUT_FILE;
        }catch (const BadHeaderException& e){
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
        std::cout << "\nConverted successfully\n";
    } else{
        std::cout << "Wrong arguments";
    }

    return 0;
}
