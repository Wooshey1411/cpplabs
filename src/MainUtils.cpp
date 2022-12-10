#include "MainUtils.h"
#include "Exceptions.h"
#include "Converters/ConvertersFactory.h"
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