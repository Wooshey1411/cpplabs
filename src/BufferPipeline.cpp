#include "BufferPipeline.h"

BufferPipeline::BufferPipeline():pos(0),currSec(0),frequency(0),endPos(0),buffer(new short[lengthOfBuffer]) {}
BufferPipeline::~BufferPipeline() {
    delete[] buffer;
}