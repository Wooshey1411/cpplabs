#include "WAVWriter.h"

WAVWriter::WAVWriter(std::string_view path):_path(path) {
    _out.open(_path,std::fstream::out | std::fstream::binary);
}


void WAVWriter::writeHeader(const Header* wavHeader) {
   WAV_HEADER h = *wavHeader;
    _out.write(reinterpret_cast<char*>(&h.headerMain),sizeof(wavHeader->headerMain));

    if(wavHeader->listExist){
        _out.write(reinterpret_cast<char*>(&h.listHeader.LIST[0]),align);
        _out.write(reinterpret_cast<char*>(&h.listHeader.listSize),sizeof(uint32_t));
        _out.write(reinterpret_cast<char*>(&h.listHeader.list[0]),wavHeader->listHeader.listSize);
    }
    _out.write(reinterpret_cast<char*>(&h.DATA[0]),align);
    _out.write(reinterpret_cast<char*>(&h.subChunk2Size),sizeof(uint32_t));
}

void WAVWriter::writeSecond(BufferPipeline* bufferPipeline) {
    if(bufferPipeline->endPos == 0) {
       _out.write(reinterpret_cast<char*>(&bufferPipeline->buffer[bufferPipeline->pos]),sizeof(uint16_t)*bufferPipeline->frequency);
    }
    else{
        _out.write(reinterpret_cast<char*>(&bufferPipeline->buffer[bufferPipeline->pos]),sizeof(uint16_t)*(bufferPipeline->endPos-bufferPipeline->pos));
    }
    bufferPipeline->pos+=bufferPipeline->frequency;
}

WAVWriter::~WAVWriter() {
    fclose(_writer);
}