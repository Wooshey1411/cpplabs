#include <string>
typedef struct INITIAL_FINAL_PARAMS{
    unsigned int initial;
    unsigned int final;
} IFParams;

typedef struct PATH_INITIAL_PARAMS{
    std::string path;
    unsigned int initial;
}PIParams;

typedef struct INITIAL_FINAL_COEFFICIENT{
    unsigned int initial;
    unsigned int final;
    unsigned int coefficient;
} IFCParams;



/*
class Params{
protected:
    unsigned int _code;
public:
    unsigned int code() {return _code;};
    Params(unsigned int code):_code(code) {};
    virtual ~Params() = default;
};

class IFParams : public Params{
public:
    unsigned int initial;
    unsigned int final;
    ~IFParams() override = default;
};

class PIPParams : public Params{
public:
    unsigned int initial;
    std::string path;
    void (*pFunction)();
    ~PIPParams() override = default;
};

class IFCParams : public Params{
public:
    unsigned int initial;
    unsigned int final;
    unsigned int coefficient;
    ~IFCParams() override = default;
};
*/
