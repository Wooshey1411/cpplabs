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
        ConfigParser configParser(files[0]);
        std::vector<std::variant<std::string, unsigned int>> params;
        try {
            configParser.parse(params);
        } catch(const BadConfigException& e) {
            std::cerr << e.what() << "\n";
            return BAD_CONFIG;
        }catch(const NoConfigException& e){
            std::cerr << e.what() << "\n";
            return NO_CONFIG_FILE;
        }

        Processor processor;
        try {
            processor.convert(files,params);
        }
        catch (const NoFileException& e){
            std::cerr << e.what();
            return NO_INPUT_FILE;
        }catch (const BadHeaderException& e){
            std::cerr << e.what();
            return BAD_HEADER;
        }

        std::cout << "\nConverted successfully\n";
    } else{
        std::cout << "Wrong arguments";
    }

    return 0;
}
