#pragma once
#include <string>
#include <memory>
#include <fstream>
#include "WAVheader.h"
#include "BufferPipeline.h"
#include "Exceptions.h"

class WAVReader{
public:
    WAVReader(std::string_view);
    void readHeader();
    void printHeader();
    const Header* getHeader();
    void setHeaderToStereo(uint32_t subchunk2size);
    bool readByFrequency(BufferPipeline*);
    bool readFullBuffer(BufferPipeline*);
    ~WAVReader();
private:
    std::fstream _in;
    std::string _path;
    Header _wavHeader;
};