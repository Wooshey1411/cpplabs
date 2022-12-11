#include "Reader.h"

Reader::Reader(std::string_view path) {
    _path = path;
    _in.open(_path, std::fstream::in | std::fstream::binary);
}


bool Reader::read(BufferPipeline& bufferPipeline,unsigned int size) {
        if(_in.eof()){
            return false;
        }
        if(bufferPipeline.pos+size >= LENGTH_OF_BUFFER){
            bufferPipeline.pos = 0;
        }

        _in.read(reinterpret_cast<char*>(&bufferPipeline.buffer[bufferPipeline.pos]),sizeof(uint16_t)*size);
        unsigned long long readed = _in.gcount() / sizeof(uint16_t);

        if(_in.eof()){
            bufferPipeline.endPos = bufferPipeline.pos + readed;
        }
        return true;
}

bool Reader::readFullBuffer(BufferPipeline & bufferPipeline) {

    if(_in.eof()){
        return false;
    }

    _in.read(reinterpret_cast<char*>(&bufferPipeline.buffer[bufferPipeline.pos]),sizeof(uint16_t)*(LENGTH_OF_BUFFER-bufferPipeline.pos));
    unsigned long long readed = _in.gcount() / sizeof(uint16_t);
    if (readed != LENGTH_OF_BUFFER-bufferPipeline.pos){
        bufferPipeline.endPos = bufferPipeline.pos + readed;
    }
    return true;
}