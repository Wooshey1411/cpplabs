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
        if(files.size() < 3){ // minimum 3 - config/in/out
            std::cerr << "low count of arguments";
            return BAD_ARGS;
        }

        ConfigParser configParser(files[0]);
        files.erase(files.begin());
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
            processor.deleteTempFiles(files[0]);
            return NO_INPUT_FILE;
        }catch (const BadHeaderException& e){
            std::cerr << e.what();
            processor.deleteTempFiles(files[0]);
            return BAD_HEADER;
        }catch (const BadConfigException& e){
            std::cerr << e.what();
            processor.deleteTempFiles(files[0]);
            return BAD_CONFIG;
        }catch(const std::bad_variant_access&){
            std::cerr << "Bad params in config";
            processor.deleteTempFiles(files[0]);
            return BAD_CONFIG;
        }
        processor.deleteTempFiles(files[0]);
        std::cout << "\nConverted successfully\n";
    } else{
        std::cout << "Wrong arguments";
    }

    return 0;
}
