#include "Writer.h"

Writer::Writer(std::string_view path) {
    _path = path;
    _out.open(_path,std::fstream::out | std::fstream::binary);
}

void Writer::write(BufferPipeline& bufferPipeline, unsigned int size) {
    if(bufferPipeline.endPos == 0) {
        _out.write(reinterpret_cast<char*>(&bufferPipeline.buffer[bufferPipeline.pos]),sizeof(uint16_t)*size);
    }
    else{
        _out.write(reinterpret_cast<char*>(&bufferPipeline.buffer[bufferPipeline.pos]),sizeof(uint16_t)*(bufferPipeline.endPos-bufferPipeline.pos));
    }
    bufferPipeline.pos+=size;
    if(bufferPipeline.pos >= LENGTH_OF_BUFFER){
        bufferPipeline.pos = 0;
    }
}