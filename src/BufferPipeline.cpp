#include "BufferPipeline.h"

inline const int SHIFT_ALIGN = 44100*3;

BufferPipeline::BufferPipeline():buffer(new short[LENGTH_OF_BUFFER]),pos(0),currSec(0),frequency(0),endPos(0) {}
BufferPipeline::~BufferPipeline() {
    delete[] buffer;
}

void BufferPipeline::shiftToBegin() {
    if(pos < SHIFT_ALIGN){
        return;
    }
    for (int i = 0; i < SHIFT_ALIGN; ++i) {
        buffer[i] = buffer[pos-SHIFT_ALIGN+i+1];
    }
    pos = SHIFT_ALIGN+1;
}