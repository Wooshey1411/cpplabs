#include "WAVWriter.h"

WAVWriter::WAVWriter(std::string_view path):_path(path) {
    _writer = fopen(_path.c_str(),"wb");
}


void WAVWriter::writeHeader(const Header* wavHeader) {
    fwrite(&wavHeader->headerMain,sizeof(wavHeader->headerMain),1,_writer);

    if(wavHeader->listExist){
        fwrite(&wavHeader->listHeader.LIST,align,1,_writer);
        fwrite(&wavHeader->listHeader.listSize,sizeof(uint32_t),1,_writer);
        fwrite(&wavHeader->listHeader.list[0],wavHeader->listHeader.listSize,1,_writer);
    }
    fwrite(&wavHeader->DATA,align,1,_writer);
    fwrite(&wavHeader->subChunk2Size,sizeof(uint32_t),1,_writer);
}

void WAVWriter::writeSecond(BufferPipeline* bufferPipeline) {
    if(bufferPipeline->endPos == 0) {
        fwrite(&(bufferPipeline->buffer[bufferPipeline->pos]), 2, bufferPipeline->frequency, _writer);
    }
    else{
        fwrite(&(bufferPipeline->buffer[bufferPipeline->pos]), 2, bufferPipeline->endPos-bufferPipeline->pos, _writer);
    }
    bufferPipeline->pos+=bufferPipeline->frequency;
}

WAVWriter::~WAVWriter() {
    fclose(_writer);
}