#pragma once
#include <iostream>
#include <utility>

enum EXCEPTIONS{
    BAD_CONFIG = 1,
    NO_CONFIG_FILE,
    NO_INPUT_FILE,
    BAD_HEADER
};

class NoFileException : public std::runtime_error{
public:
    NoFileException() : std::runtime_error("File doesn't exist") {};
    NoFileException(const std::string& msg): std::runtime_error(msg.c_str()){};
};

class BadHeaderException : public std::runtime_error{
public:
    BadHeaderException() :std::runtime_error("Header corrupted") {};
    BadHeaderException(const std::string& msg): std::runtime_error(msg.c_str()){};
};

class NoConfigException : public std::runtime_error{
public:
    NoConfigException() :std::runtime_error("Config file doesn't exist") {};
    NoConfigException(const std::string& msg): std::runtime_error(msg.c_str()){};
};

class BadConfigException : public std::runtime_error{
public:
    BadConfigException() :std::runtime_error("Config corrupted") {};
    BadConfigException(const std::string& msg): std::runtime_error(msg.c_str()){};
};