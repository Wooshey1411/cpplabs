#pragma once
#include <cstdint>
inline const uint32_t DEFAULT_FREQUENCY = 44100;
inline const uint16_t COUNT_OF_BITS_IN_BYTE = 8;
inline const unsigned char align = 4;

struct HeaderMain{
    char RIFF[align]; // RIFF header
    uint32_t chunkSize; // RIFF chunk size
    char WAVE[align]; // WAVE header
    char fmt[align]; // fmt header
    uint32_t subChunk1Size; // size of the fmt chunk
    uint16_t audioFormat; // audio format: default 1 - PCM
    uint16_t countOfChannels; // 1 - mono, 2 - stereo
    uint32_t samplesPerSec; // Frequency in Hz (sample rate)
    uint32_t bytesPerSec; // bytes per second (byte rate)
    uint16_t blockAlign; // 2 - 16-bit mono, 4 - 16-bit stereo
    uint16_t bitsPerSample; // count of bits per sample;
};

struct ListHeader{
    char LIST[align]; // LIST header;
    uint32_t listSize; // count of bytes in LIST
    unsigned char* list; // list data
} ;

typedef struct WAV_HEADER{
    HeaderMain headerMain;
    ListHeader listHeader;
    bool listExist;
    char DATA[align]; // DATA header;
    uint32_t subChunk2Size; // sampled data length
} Header;


