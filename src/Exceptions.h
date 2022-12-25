#include <utility>
#include <stdexcept>

class BadLineException : public std::invalid_argument{
public:
    BadLineException() : std::invalid_argument("Bad line") {};
    BadLineException(const std::string& msg): std::invalid_argument(msg.c_str()){};
};

class BadFileException : public std::runtime_error{
public:
    BadFileException() : std::runtime_error("File doesn't exist") {};
    BadFileException(const std::string& msg): std::runtime_error(msg.c_str()){};
};

class BadPointerException : public std::logic_error{
public:
    BadPointerException() : std::logic_error("Pointer is null") {};
    BadPointerException(const std::string& msg): std::logic_error(msg.c_str()){};
};
