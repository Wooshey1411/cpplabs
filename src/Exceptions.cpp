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