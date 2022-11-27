#pragma once
#include "Converter.h"
#include "../WAVReader.h"

class MuteConverter : public Converter{
public:
    void convert(void* params, BufferPipeline* buffer) override;
    ~MuteConverter() override = default;
};

class MixConverter : public Converter{
public:
    MixConverter();
    void convert(void* params, BufferPipeline* buffer) override;
    ~MixConverter() override = default;
private:
    WAVReader* _reader;
    BufferPipeline _bufferPipeline;
    bool _isInitialized;
    bool _isFinished;
};

class DistortionConverter : public Converter{
public:
    void convert(void* params, BufferPipeline* buffer) override;
    ~DistortionConverter() override = default;
};