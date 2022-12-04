#include "Converters.h"
#include "../Params/Params.h"
#include <cmath>
#include <iomanip>

inline const unsigned short BASS_BOOSTED_VOLUME = 1000;
inline const unsigned short BASS_BOOSTED_VOLUME_COEFFICIENT = 16;

void MuteConverter::convert(std::shared_ptr<Params> params, BufferPipeline* buffer) {
    auto initial = std::any_cast<unsigned int>(params->getParams(1));
    auto final = std::any_cast<unsigned int>(params->getParams(2));
    if(buffer->currSec >= initial && buffer->currSec <= final){
        for (int i = 0; i < buffer->frequency; ++i) {
            buffer->buffer[buffer->pos+i] = 0;
        }
    }
}

void MuteConverter::printDescription(){
    std::cout << std::setw(10) << std::cout.fill() << "mute [INITIAL_SECOND] [FINAL_SECOND]\n";
    std::cout << std::setw(15) << std::cout.fill() << "mute audio from INITIAL_SECOND to FINAL_SECOND\n\n";
}

MixConverter::MixConverter():_isInitialized(false),_isFinished(false) {}

void MixConverter::convert(std::shared_ptr<Params> params, BufferPipeline *buffer) {
    auto path = std::any_cast<std::string>(params->getParams(1));
    auto initial = std::any_cast<unsigned int>(params->getParams(2));

    if(!_isInitialized){
        _streamLinker = new StreamLinker(path);
        _streamLinker->fillBuffer(&_bufferPipeline);
        _isInitialized = true;
    }

    if(buffer->currSec >= initial && !_isFinished){
        bool isOverflow = false;
        unsigned int counter = 0;
        for (int i = 0; i < buffer->frequency; ++i) {
            if(_bufferPipeline.endPos != 0 && _bufferPipeline.pos+i == _bufferPipeline.endPos){
                _isFinished = true;
                break;
            }
            if(_bufferPipeline.pos+i == LENGTH_OF_BUFFER){
                isOverflow = true;
                _bufferPipeline.pos = 0;
                _streamLinker->fillBuffer(&_bufferPipeline);
            }
            buffer->buffer[buffer->pos+i] = (buffer->buffer[buffer->pos+i]/2 + _bufferPipeline.buffer[_bufferPipeline.pos+i]/2);
            if(isOverflow){
                counter++;
            }
        }
        if(isOverflow){
            _bufferPipeline.pos += counter;
        } else {
            _bufferPipeline.pos += buffer->frequency;
        }
    }
}

void MixConverter::printDescription(){
    std::cout << std::setw(10) << std::cout.fill() << "mix [#NUMBER] [INITIAL_SECOND]\n";
    std::cout << std::setw(15) << std::cout.fill() << "mix input audio file with another by NUMBER in order of indication in arguments\n";
    std::cout << std::setw(15) << std::cout.fill() << "from INITIAL_SECOND to end of file\n\n";
}

void BassBoostedConverter::convert(std::shared_ptr<Params> params, BufferPipeline *buffer) {
    auto initial = std::any_cast<unsigned int>(params->getParams(1));
    auto final = std::any_cast<unsigned int>(params->getParams(2));
    if(buffer->currSec >= initial && buffer->currSec <= final){
        for (int i = 0; i < buffer->frequency; ++i) {
            if(buffer->buffer[buffer->pos+i] > BASS_BOOSTED_VOLUME){
                buffer->buffer[buffer->pos+i]=BASS_BOOSTED_VOLUME;
            }
            if(buffer->buffer[buffer->pos+i] < -BASS_BOOSTED_VOLUME){
                buffer->buffer[buffer->pos+i] = -BASS_BOOSTED_VOLUME;
            }
            buffer->buffer[buffer->pos+i]*=BASS_BOOSTED_VOLUME_COEFFICIENT;
        }
    }
}

void BassBoostedConverter::printDescription(){
    std::cout << std::setw(10) << std::cout.fill() << "bassBoosted [INITIAL_SECOND] [FINAL_SECOND]\n";
    std::cout << std::setw(15) << std::cout.fill() << "make bass boosted effect from INITIAL_SECOND to FINAL_SECOND\n\n";
}

DistortionConverter::DistortionConverter():_isFinished(true),_extremumPos(0),_maxV(0) {}

bool sign (int num){
    if (num >= 0) {
        return true;
    }
    else{
        return false;
    }
}

void DistortionConverter::convert(std::shared_ptr<Params> params, BufferPipeline* buffer) {
    auto initial = std::any_cast<unsigned int>(params->getParams(1));
    auto final = std::any_cast<unsigned int>(params->getParams(2));
    auto coefficient = std::any_cast<unsigned int>(params->getParams(3));

    if(buffer->currSec >= initial && buffer->currSec <= final){
        double coeff = 1.0-coefficient*1.0/100;
        for (int i = 1; i < buffer->frequency; ++i) {
            if(abs(buffer->buffer[buffer->pos+i-1]) > abs(buffer->buffer[buffer->pos+i]) || !_isFinished){
                if(_isFinished) {
                    _extremumPos = buffer->pos + i - 1;
                }
                    _maxV = static_cast<short>(round(abs(buffer->buffer[_extremumPos]) * coeff));
                    unsigned int inPos = _extremumPos;
                    unsigned int fiPos = _extremumPos;
                    buffer->buffer[_extremumPos] = _maxV;
                    bool leftDone=false;
                    bool rightDone = false;
                    while(true){
                        _isFinished = false;
                        if(inPos > 1 && abs(buffer->buffer[inPos-1]) > _maxV && !leftDone && abs(buffer->buffer[inPos-1]) > abs(buffer->buffer[inPos-2])
                        && sign(buffer->buffer[inPos]) == sign(buffer->buffer[inPos-1])) {

                            if(buffer->buffer[inPos-1] < 0) {
                                buffer->buffer[inPos - 1] = static_cast<short>(-_maxV);
                            } else {
                                buffer->buffer[inPos - 1] = static_cast<short>(_maxV);
                            }
                            inPos--;
                        } else{
                            leftDone = true;
                        }

                        if(fiPos < LENGTH_OF_BUFFER-1 && fiPos < (buffer->pos+buffer->frequency) && abs(buffer->buffer[fiPos+1]) >= _maxV && !rightDone
                        && abs(buffer->buffer[fiPos+1]) > abs(buffer->buffer[fiPos+2]) && sign(buffer->buffer[fiPos]) == sign(buffer->buffer[fiPos+1])){
                            if(buffer->buffer[fiPos+1] < 0) {
                                buffer->buffer[fiPos + 1] = static_cast<short>(-_maxV);
                            }
                            else{
                                buffer->buffer[fiPos + 1] = static_cast<short>(_maxV);
                            }
                            fiPos++;
                        } else{
                            rightDone = true;
                        }

                        if(leftDone && rightDone) {
                            if(fiPos < buffer->pos+buffer->frequency) {
                                _isFinished = true;
                            }
                            break;
                        }
                    }
            }
        }
    }
}

void DistortionConverter::printDescription(){
    std::cout << std::setw(10) << std::cout.fill() << "distortion [INITIAL_SECOND] [FINAL_SECOND] [COEFFICIENT]\n";
    std::cout << std::setw(15) << std::cout.fill() << "make distortion effect from INITIAL_SECOND to FINAL_SECOND\n";
    std::cout << std::setw(15) << std::cout.fill() << "COEFFICIENT - level of cutting of wave in percents\n\n";
}