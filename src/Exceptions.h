#pragma once
#include <iostream>

enum EXCEPTIONS{
    BAD_CONFIG = 1,
    NO_CONFIG_FILE,
    NO_INPUT_FILE,
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

class ConfigException : public BaseException{
public:
    ConfigException();
    ConfigException(std::string message);
    std::string what() override;
};

class NoConfigException final : public ConfigException{
public:
    NoConfigException() : ConfigException() {text = "Config file doesn't exist";}
    NoConfigException(std::string message) : ConfigException(message){};
    std::string what() override;
};

class BadConfigException final : public ConfigException{
public:
    BadConfigException() : ConfigException() {text = "Config corrupted";}
    BadConfigException(std::string message) : ConfigException(message){};
    std::string what() override;
};