#include "Converters.h"
#include "../Params.h"
#include <cmath>

inline const unsigned short BASS_BOOSTED_VOLUME = 1000;
inline const unsigned short BASS_BOOSTED_VOLUME_COEFFICIENT = 16;

void MuteConverter::convert(void* params, BufferPipeline* buffer) {
    auto* ifParams = static_cast<IFParams*>(params);
    if(buffer->currSec >= ifParams->initial && buffer->currSec <= ifParams->final){
        for (int i = 0; i < buffer->frequency; ++i) {
            buffer->buffer[buffer->pos+i] = 0;
        }
    }
}

MixConverter::MixConverter():_isInitialized(false),_isFinished(false) {}

void MixConverter::convert(void *params, BufferPipeline *buffer) {
    auto* piparams = static_cast<PIParams*>(params);

    if(!_isInitialized){
        _reader = new WAVReader(piparams->path);
        _reader->readHeader();
        _reader->readFullBuffer(&_bufferPipeline);
        _isInitialized = true;
    }

    if(buffer->currSec >= piparams->initial && !_isFinished){
        bool isOverflow = false;
        unsigned int counter = 0;
        for (int i = 0; i < buffer->frequency; ++i) {
            if(_bufferPipeline.endPos != 0 && _bufferPipeline.pos+i == _bufferPipeline.endPos){
                _isFinished = true;
                break;
            }
            if(_bufferPipeline.pos+i == lengthOfBuffer){
                isOverflow = true;
                _bufferPipeline.pos = 0;
                _reader->readFullBuffer(&_bufferPipeline);
            }
            buffer->buffer[buffer->pos+i] = (buffer->buffer[buffer->pos+i]/2 + _bufferPipeline.buffer[_bufferPipeline.pos+i]/4);
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

void BassBoostedConverter::convert(void *params, BufferPipeline *buffer) {
    auto* ifParams = static_cast<IFParams*>(params);
    if(buffer->currSec >= ifParams->initial && buffer->currSec <= ifParams->final){
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

DistortionConverter::DistortionConverter():_isFinished(true),_extremumPos(0),_maxV(0) {}

void DistortionConverter::convert(void* params, BufferPipeline* buffer) {
    auto* ifcParams = static_cast<IFCParams*>(params);
    if(buffer->currSec >= ifcParams->initial && buffer->currSec <= ifcParams->final){
        unsigned int inPos = 0;
        unsigned int extremumPos = 0;
        unsigned int fiPos = 0;
        float coeff = 1.0-ifcParams->coefficient*1.0/100;
        for (int i = 1; i < buffer->frequency; ++i) {
            if(abs(buffer->buffer[buffer->pos+i-1]) > abs(buffer->buffer[buffer->pos+i]) || !_isFinished){
                    if(_isFinished) {
                        _extremumPos = buffer->pos + i - 1;
                    }
                    _maxV = static_cast<short>(roundf(abs(buffer->buffer[extremumPos]) * coeff));
                    inPos = _extremumPos;
                    fiPos = _extremumPos;
                    buffer->buffer[_extremumPos] = _maxV;
                    bool leftDone=false;
                    bool rightDone = false;
                    while(true){
                        _isFinished = false;
                        if(inPos > 1 && inPos >= (_extremumPos - buffer->frequency) && abs(buffer->buffer[inPos-1]) > _maxV && !leftDone && abs(buffer->buffer[inPos-1]) > abs(buffer->buffer[inPos-2])) {

                            if(buffer->buffer[inPos-1] < 0) {
                                buffer->buffer[inPos - 1] = static_cast<short>(-_maxV);
                            } else {
                                buffer->buffer[inPos - 1] = static_cast<short>(_maxV);
                            }
                            inPos--;
                        } else{
                            leftDone = true;
                        }

                        if(fiPos < lengthOfBuffer-1 && fiPos <= (_extremumPos + buffer->frequency) && abs(buffer->buffer[fiPos+1]) > _maxV && !rightDone && abs(buffer->buffer[fiPos+1]) > abs(buffer->buffer[fiPos+2])){
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
                            if(fiPos > (_extremumPos + buffer->frequency) &&  inPos < (_extremumPos - buffer->frequency)){
                                _isFinished = true;
                            }
                            break;
                        }


                    }


            }


        }


    }
}