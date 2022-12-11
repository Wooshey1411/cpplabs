#include "WAVReader.h"

void WAVReader::readHeader() {
    if(!_in.is_open()){
        throw NoFileException();
    }
    _in.read(reinterpret_cast<char*>(&_wavHeader.headerMain),sizeof(_wavHeader.headerMain));

    if(_wavHeader.headerMain.RIFF[0] != 'R' ||_wavHeader.headerMain.RIFF[1] != 'I'
    || _wavHeader.headerMain.RIFF[2] != 'F' || _wavHeader.headerMain.RIFF[3] != 'F'){
        throw BadHeaderException();
    }

    if(_wavHeader.headerMain.fmt[0] != 'f' ||_wavHeader.headerMain.fmt[1] != 'm'
       || _wavHeader.headerMain.fmt[2] != 't' || _wavHeader.headerMain.fmt[3] != ' '){
        throw BadHeaderException();
    }

    if(_wavHeader.headerMain.subChunk1Size != sizeof(uint16_t)*COUNT_OF_BITS_IN_BYTE || _wavHeader.headerMain.audioFormat != 1
    || _wavHeader.headerMain.samplesPerSec != DEFAULT_FREQUENCY || _wavHeader.headerMain.countOfChannels != 1 || _wavHeader.headerMain.blockAlign != 2){
        throw BadHeaderException("unsupportable format");
    }

    char checker;
     long long pos = _in.tellg();

    _in.read(reinterpret_cast<char*>(&checker),sizeof(char));
    _in.seekg(pos);
    if(checker != 'L' && checker != 'd') {
        throw BadHeaderException();
    }

    if(checker == 'L'){

       _in.read(reinterpret_cast<char*>(&_wavHeader.listHeader.LIST[0]),align);

        if(_wavHeader.listHeader.LIST[0] != 'L' || _wavHeader.listHeader.LIST[1] != 'I'
        || _wavHeader.listHeader.LIST[2] != 'S' || _wavHeader.listHeader.LIST[3] != 'T'){
            throw BadHeaderException();
        }
        _in.read(reinterpret_cast<char*>(&_wavHeader.listHeader.listSize),sizeof(uint32_t));
        _wavHeader.listExist = true;
        _wavHeader.listHeader.list = new unsigned char[_wavHeader.listHeader.listSize+1];
        _in.read(reinterpret_cast<char*>(&_wavHeader.listHeader.list[0]),_wavHeader.listHeader.listSize);
    }else{
        _wavHeader.listExist = false;
    }

   _in.read(reinterpret_cast<char*>(&_wavHeader.DATA[0]),align);
    if(_wavHeader.DATA[0] != 'd' || _wavHeader.DATA[1] != 'a'
       || _wavHeader.DATA[2] != 't' || _wavHeader.DATA[3] != 'a'){
        throw BadHeaderException();
    }
    _in.read(reinterpret_cast<char*>(&_wavHeader.subChunk2Size),sizeof(uint32_t));
}

void WAVReader::printHeader() {
    std::cout << "RIFF header                :" << _wavHeader.headerMain.RIFF[0]
              << _wavHeader.headerMain.RIFF[1]
              << _wavHeader.headerMain.RIFF[2]
              << _wavHeader.headerMain.RIFF[3] << "\n";

    std::cout << "WAVE header                :" << _wavHeader.headerMain.WAVE[0]
              << _wavHeader.headerMain.WAVE[1]
              << _wavHeader.headerMain.WAVE[2]
              << _wavHeader.headerMain.WAVE[3]
              << "\n";

    std::cout << "FMT                        :" << _wavHeader.headerMain.fmt[0]
              << _wavHeader.headerMain.fmt[1]
              << _wavHeader.headerMain.fmt[2]
              << _wavHeader.headerMain.fmt[3]
              << "\n";

    std::cout << "Data(chunk) size           :" <<_wavHeader.headerMain.chunkSize << "\n";

    // Display the sampling Rate form the header
    std::cout << "Sampling Rate              :" << _wavHeader.headerMain.samplesPerSec << "\n";
    std::cout << "Number of bits used        :" << _wavHeader.headerMain.bitsPerSample << "\n";
    std::cout << "Number of channels         :" << _wavHeader.headerMain.countOfChannels << "\n";
    std::cout << "Number of bytes per second :" << _wavHeader.headerMain.bytesPerSec << "\n";
    std::cout << "subchunk size              :" << _wavHeader.headerMain.subChunk1Size << "\n";
    std::cout << "Data length                :" << _wavHeader.subChunk2Size << "\n";
    std::cout << "Audio Format               :" << _wavHeader.headerMain.audioFormat << "\n";

    std::cout << "Block align                :" << _wavHeader.headerMain.blockAlign << "\n";

    if(_wavHeader.listExist) {
        std::cout << "LIST header                :" << _wavHeader.listHeader.LIST[0]
                  << _wavHeader.listHeader.LIST[1]
                  << _wavHeader.listHeader.LIST[2]
                  << _wavHeader.listHeader.LIST[3] << "\n";
        std::cout << "list count                 :" << _wavHeader.listHeader.listSize << "\n";
        std::cout << "list                       :" << "\n";
        for (unsigned int i = 0; i < _wavHeader.listHeader.listSize; ++i) {
            std::cout << _wavHeader.listHeader.list[i];
        }
        std::cout << std::endl;
    }

}

const Header *WAVReader::getHeader() {
    return &_wavHeader;
}

WAVReader::~WAVReader() {
    if(_wavHeader.listExist)
        delete[] _wavHeader.listHeader.list;
}

