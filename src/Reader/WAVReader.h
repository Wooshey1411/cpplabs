#pragma once
#include "../WAVheader.h"
#include "../Exceptions.h"
#include "Reader.h"

class WAVReader : public Reader{
public:
    WAVReader(std::string_view path) : Reader(path){};
    void readHeader();
    void printHeader();
    const Header* getHeader();
   // bool readByFrequency(BufferPipeline*);
  //  bool readFullBuffer(BufferPipeline*);
    ~WAVReader() override;
private:
   // std::fstream _in;
  //  std::string _path;
    Header _wavHeader;
};