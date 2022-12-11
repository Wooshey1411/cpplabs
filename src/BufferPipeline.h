#pragma once

inline const int LENGTH_OF_BUFFER = 44100*60*5*2*2; // 44100 - average frequency, 2 - sizeof(uint_16), 60 - seconds in minute, 2 - minutes


class BufferPipeline{
public:
    short* buffer;
    unsigned long long pos;
    unsigned long long endPos;
    void shiftToBegin();
    BufferPipeline();
    ~BufferPipeline();
};