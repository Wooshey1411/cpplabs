#pragma once

inline const int lengthOfBuffer = 44100*60; // 44100 - average frequency, 4 - sizeof(int), 60 - seconds in minute

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