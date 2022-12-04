#pragma once
#include <string>
#include <any>
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


class Params{
protected:
    unsigned int _countOfParams;
public:
    Params();
    unsigned int countOfParams() const {return _countOfParams;};
    virtual void setParams(unsigned int pos, std::any arg) = 0;
    virtual std::any getParams(unsigned int pos) = 0;
    virtual ~Params() = default;
};

class IFParamsC : public Params{
public:
    IFParamsC();
    void setParams(unsigned int pos, std::any arg) override;
    std::any getParams(unsigned int pos) override;
    ~IFParamsC() override = default;
private:
    unsigned int _initial;
    unsigned int _final;
};

class PIPParamsC : public Params{
public:
    PIPParamsC();
    void setParams(unsigned int pos, std::any arg) override;
    std::any getParams(unsigned int pos) override;
    ~PIPParamsC() override = default;
private:
    std::string _path;
    unsigned int _initial;
};

class IFCParamsC : public Params{
public:
    IFCParamsC();
    void setParams(unsigned int pos, std::any arg) override;
    std::any getParams(unsigned int pos) override;
    ~IFCParamsC() override = default;
private:
    unsigned int _initial;
    unsigned int _final;
    unsigned int _coefficient;
};
