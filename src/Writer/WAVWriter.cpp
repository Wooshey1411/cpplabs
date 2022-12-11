#include "WAVWriter.h"

/*WAVWriter::WAVWriter(std::string_view unnamed, std::string_view path) : Writer(unnamed) {
    _path = path;
    _out.open(_path,std::fstream::out | std::fstream::binary);
}*/


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
