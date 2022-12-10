#pragma once
#include "Converter.h"
#include "../WAVReader.h"
#include "StreamLinker.h"

class MuteConverter : public Converter{
public:
    void convert(std::vector<std::variant<std::string,unsigned int>> &params,BufferPipeline*) override;
    unsigned int getCountOfParams() override;
    void printDescription() override;
    ~MuteConverter() override = default;
};

class MixConverter : public Converter{
public:
    MixConverter();
    void convert(std::vector<std::variant<std::string,unsigned int>> &params,BufferPipeline*) override;
    unsigned int getCountOfParams() override;
    void printDescription() override;
    ~MixConverter() override = default;
private:
    StreamLinker* _streamLinker;
    BufferPipeline _bufferPipeline;
    bool _isInitialized;
    bool _isFinished;
};

class BassBoostedConverter : public Converter{
public:
    void convert(std::vector<std::variant<std::string,unsigned int>> &params,BufferPipeline*) override;
    unsigned int getCountOfParams() override;
    void printDescription() override;
    ~BassBoostedConverter() override = default;
};

class DistortionConverter : public Converter{
public:
    DistortionConverter();
    void convert(std::vector<std::variant<std::string,unsigned int>> &params,BufferPipeline*) override;
    unsigned int getCountOfParams() override;
    void printDescription() override;
    ~DistortionConverter() override = default;
private:
    short _maxV;
    bool _isFinished;
    unsigned long long _extremumPos;
};