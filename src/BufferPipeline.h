#pragma once

inline const int LENGTH_OF_BUFFER = 44100*60*4; // 44100 - average frequency, 4 - sizeof(int), 60 - seconds in minute

class BufferPipeline{
public:
    short* buffer;
    unsigned int pos;
    unsigned int currSec;
    unsigned int frequency;
    unsigned int endPos;
    BufferPipeline();
    ~BufferPipeline();
};