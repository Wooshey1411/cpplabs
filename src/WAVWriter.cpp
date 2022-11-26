#include "WAVWriter.h"

WAVWriter::WAVWriter(std::string_view path):_path(path) {
    _writer = fopen(_path.c_str(),"wb");
}


void WAVWriter::writeHeader(const Header* wavHeader) {
    fwrite(&wavHeader->headerMain,sizeof(wavHeader->headerMain),1,_writer);

    if(wavHeader->listExist){
        fwrite(&wavHeader->listHeader.LIST,align,1,_writer);
        fwrite(&wavHeader->listHeader.list[0],wavHeader->listHeader.listSize,1,_writer);
    }
    fwrite(&wavHeader->DATA,align,1,_writer);
    fwrite(&wavHeader->subChunk2Size,sizeof(uint32_t),1,_writer);
}