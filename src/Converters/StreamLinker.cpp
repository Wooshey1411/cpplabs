#include "StreamLinker.h"
#include "../Exceptions.h"

StreamLinker::StreamLinker(const std::string& path) {
    _reader = new WAVReader(path);
    _reader->readHeader();
}

void StreamLinker::fillBuffer(BufferPipeline* bufferPipeline) {
    _reader->readFullBuffer(bufferPipeline);
}
