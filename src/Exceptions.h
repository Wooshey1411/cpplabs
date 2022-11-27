#pragma once
#include <iostream>

enum EXCEPTIONS{
    BAD_CONFIG = 1,
    NO_FILE,
    BAD_HEADER
};


class BaseException : public std::exception {
protected:
    std::string text;
public:
    virtual std::string what() = 0;
};

class WAVReaderException : public BaseException{
public:
    WAVReaderException();
    WAVReaderException(std::string message);
    std::string what() override;
};

class NoFileException final : public WAVReaderException{
public:
    NoFileException() : WAVReaderException() {text = "File doesn't exist";}
    NoFileException(std::string message) : WAVReaderException(message){};
    std::string what() override;
};

class BadHeaderException final : public WAVReaderException{
public:
    BadHeaderException() : WAVReaderException() {text = "Header corrupted";}
    BadHeaderException(std::string message) : WAVReaderException(message){};
    std::string what() override;
};
