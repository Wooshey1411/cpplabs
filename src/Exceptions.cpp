#include "Exceptions.h"

#include <utility>

WAVReaderException::WAVReaderException(){
    text = "Error. File corrupted";
}
WAVReaderException::WAVReaderException(std::string message) {
    text = std::move(message);
}

std::string WAVReaderException::what() {
    return text;
}

std::string NoFileException::what() {
    return text;
}

std::string BadHeaderException::what() {
    return text;
}

ConfigException::ConfigException() {
    text = "Error. File corrupted";
}

ConfigException::ConfigException(std::string message) {
    text = std::move(message);
}

std::string ConfigException::what() {
    return text;
}

std::string NoConfigException::what() {
    return text;
}

std::string BadConfigException::what() {
    return text;
}