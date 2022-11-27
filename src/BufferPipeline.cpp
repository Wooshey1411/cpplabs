#include "BufferPipeline.h"

BufferPipeline::BufferPipeline():pos(0),currSec(0),frequency(0),endPos(0),buffer(new short[LENGTH_OF_BUFFER]) {}
BufferPipeline::~BufferPipeline() {
    delete[] buffer;
}