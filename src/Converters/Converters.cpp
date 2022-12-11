#include "Converters.h"
#include <cmath>
#include <iomanip>

inline const unsigned short BASS_BOOSTED_VOLUME = 1000;
inline const unsigned short BASS_BOOSTED_VOLUME_COEFFICIENT = 16;

unsigned int MuteConverter::getCountOfParams() {return 2;} // initial,final

void MuteConverter::convert(std::vector<std::variant<std::string,unsigned int>> &params,std::vector<std::unique_ptr<Reader>> &streams,Writer &out, uint32_t frequency) {
    if(params.size() < getCountOfParams()){
        throw BadConfigException("Wrong config");
    }
    auto initial = std::get<unsigned int>(params[0]);
    auto final = std::get<unsigned int>(params[1]);
    BufferPipeline inBuff;
    BufferPipeline outBuff;
    unsigned int seconds = 0;

    while(streams[0]->read(inBuff,frequency)) {
        if (seconds >= initial && seconds <= final) {
            for (unsigned int i = 0; i < frequency; ++i) {
                outBuff.buffer[outBuff.pos + i] = 0;
            }
        }else{
            for (unsigned int i = 0; i < frequency; ++i) {
                outBuff.buffer[outBuff.pos + i] = inBuff.buffer[inBuff.pos+i];
            }
        }
        seconds++;

        if(inBuff.endPos == 0) {
            out.write(outBuff, frequency);
        } else{
            out.write(outBuff,static_cast<unsigned int>(inBuff.endPos-inBuff.pos));
        }
    }
}

void MuteConverter::printDescription(){
    std::cout << std::setw(10) << std::cout.fill() << "mute [INITIAL_SECOND] [FINAL_SECOND]\n";
    std::cout << std::setw(15) << std::cout.fill() << "mute audio from INITIAL_SECOND to FINAL_SECOND\n\n";
}

MixConverter::MixConverter():_isFinished(false) {}

unsigned int MixConverter::getCountOfParams() {return 2;} // initial, number of stream

void MixConverter::convert(std::vector<std::variant<std::string,unsigned int>> &params,std::vector<std::unique_ptr<Reader>> &streams,Writer &out, uint32_t frequency) {
    if(params.size() < getCountOfParams()){
        throw BadConfigException("Wrong config");
    }
    auto stream = std::get<unsigned int>(params[0]);
    auto initial = std::get<unsigned int>(params[1]);

    BufferPipeline inBuff;
    BufferPipeline outBuff;
    BufferPipeline secondBuf;
    unsigned int seconds = 0;

    if(stream >= streams.size()){
        throw BadConfigException("Wrong number of stream");
    }

    streams[stream]->readFullBuffer(secondBuf);

    while(streams[0]->read(inBuff,frequency)) {

        if (seconds >= initial && !_isFinished) {
            bool isOverflow = false;
            unsigned int counter = 0;
            for (unsigned int i = 0; i < frequency; ++i) {
                if (secondBuf.endPos != 0 && secondBuf.pos + i == secondBuf.endPos) {
                    _isFinished = true;
                    break;
                }
                if (secondBuf.pos + i == LENGTH_OF_BUFFER) {
                    isOverflow = true;
                    secondBuf.pos = 0;
                    streams[stream]->readFullBuffer(secondBuf);
                }
                outBuff.buffer[outBuff.pos + i] = (inBuff.buffer[inBuff.pos + i] / 2 +
                                                   secondBuf.buffer[secondBuf.pos + i] / 2);
                if (isOverflow) {
                    counter++;
                }
            }
            if (isOverflow) {
                secondBuf.pos += counter;
            } else {
                secondBuf.pos += frequency;
            }
        } else{
            for (unsigned int i = 0; i < frequency; ++i) {
                outBuff.buffer[outBuff.pos + i] = inBuff.buffer[inBuff.pos+i];
            }
        }
        seconds++;
        if(inBuff.endPos == 0) {
            out.write(outBuff, frequency);
        } else{
            out.write(outBuff,static_cast<unsigned int>(inBuff.endPos-inBuff.pos));
        }
    }

}

void MixConverter::printDescription(){
    std::cout << std::setw(10) << std::cout.fill() << "mix [$NUMBER] [INITIAL_SECOND]\n";
    std::cout << std::setw(15) << std::cout.fill() << "mix input audio file with another by NUMBER in order of indication in arguments\n";
    std::cout << std::setw(15) << std::cout.fill() << "from INITIAL_SECOND to end of file\n\n";
}

unsigned int BassBoostedConverter::getCountOfParams() {return 2;} // initial,final

void BassBoostedConverter::convert(std::vector<std::variant<std::string,unsigned int>> &params,std::vector<std::unique_ptr<Reader>> &streams,Writer &out, uint32_t frequency) {
    if(params.size() < getCountOfParams()){
        throw BadConfigException("Wrong config");
    }
    auto initial = std::get<unsigned int>(params[0]);
    auto final = std::get<unsigned int>(params[1]);
    BufferPipeline inBuff;
    BufferPipeline outBuff;
    unsigned int seconds = 0;

    while(streams[0]->read(inBuff,frequency)) {

        if (seconds >= initial && seconds <= final) {
            for (unsigned int i = 0; i < frequency; ++i) {
                if (inBuff.buffer[inBuff.pos + i] > BASS_BOOSTED_VOLUME) {
                    outBuff.buffer[outBuff.pos + i] = BASS_BOOSTED_VOLUME;
                }
                if (inBuff.buffer[inBuff.pos + i] < -BASS_BOOSTED_VOLUME) {
                    outBuff.buffer[outBuff.pos + i] = -BASS_BOOSTED_VOLUME;
                }
                outBuff.buffer[outBuff.pos + i] *= BASS_BOOSTED_VOLUME_COEFFICIENT;
            }
        }else{
            for (unsigned int i = 0; i < frequency; ++i) {
                outBuff.buffer[outBuff.pos + i] = inBuff.buffer[inBuff.pos+i];
            }
        }
        seconds++;
        if(inBuff.endPos == 0) {
            out.write(outBuff, frequency);
        } else{
            out.write(outBuff,static_cast<unsigned int>(inBuff.endPos-inBuff.pos));
        }
    }
}

void BassBoostedConverter::printDescription(){
    std::cout << std::setw(10) << std::cout.fill() << "bassBoosted [INITIAL_SECOND] [FINAL_SECOND]\n";
    std::cout << std::setw(15) << std::cout.fill() << "make bass boosted effect from INITIAL_SECOND to FINAL_SECOND\n\n";
}

unsigned int DistortionConverter::getCountOfParams() { return 3;} // initial,final,coefficient

DistortionConverter::DistortionConverter():_maxV(0),_isFinished(true),_extremumPos(0) {}

bool sign (int num){
    if (num >= 0) {
        return true;
    }
    else{
        return false;
    }
}

void DistortionConverter::convert(std::vector<std::variant<std::string,unsigned int>> &params,std::vector<std::unique_ptr<Reader>> &streams,Writer &out, uint32_t frequency) {
    if(params.size() < 3){
        throw BadConfigException("Wrong config");
    }
    auto initial = std::get<unsigned int>(params[0]);
    auto final = std::get<unsigned int>(params[1]);
    auto coefficient = std::get<unsigned int>(params[2]);

    BufferPipeline inBuff;
    BufferPipeline outBuff;
    unsigned int seconds = 0;
    while(streams[0]->read(inBuff,frequency)){
        for (unsigned int i = 0; i < frequency; ++i) {
            outBuff.buffer[outBuff.pos+i] = inBuff.buffer[inBuff.pos+i];
        }
    if(seconds >= initial && seconds <= final){
        double coeff = 1.0-coefficient*1.0/100;
        for (unsigned int i = 1; i < frequency; ++i) {
            if (abs(outBuff.buffer[outBuff.pos + i - 1]) > abs(outBuff.buffer[outBuff.pos + i]) || !_isFinished) {
                if (_isFinished) {
                    _extremumPos = outBuff.pos + i - 1;
                }
                _maxV = static_cast<short>(round(abs(outBuff.buffer[_extremumPos]) * coeff));
                unsigned long long inPos = _extremumPos;
                unsigned long long fiPos = _extremumPos;
                outBuff.buffer[_extremumPos] = _maxV;
                bool leftDone = false;
                bool rightDone = false;
                while (true) {
                    _isFinished = false;
                    if (inPos > 1 && abs(outBuff.buffer[inPos - 1]) > _maxV && !leftDone &&
                        abs(outBuff.buffer[inPos - 1]) > abs(outBuff.buffer[inPos - 2])
                        && sign(outBuff.buffer[inPos]) == sign(outBuff.buffer[inPos - 1])) {

                        if (outBuff.buffer[inPos - 1] < 0) {
                            outBuff.buffer[inPos - 1] = static_cast<short>(-_maxV);
                        } else {
                            outBuff.buffer[inPos - 1] = static_cast<short>(_maxV);
                        }
                        inPos--;
                    } else {
                        leftDone = true;
                    }

                    if (fiPos < LENGTH_OF_BUFFER - 1 && fiPos < (outBuff.pos + frequency) &&
                        abs(outBuff.buffer[fiPos + 1]) >= _maxV && !rightDone
                        && abs(outBuff.buffer[fiPos + 1]) > abs(outBuff.buffer[fiPos + 2]) &&
                        sign(outBuff.buffer[fiPos]) == sign(outBuff.buffer[fiPos + 1])) {
                        if (outBuff.buffer[fiPos + 1] < 0) {
                            outBuff.buffer[fiPos + 1] = static_cast<short>(-_maxV);
                        } else {
                            outBuff.buffer[fiPos + 1] = static_cast<short>(_maxV);
                        }
                        fiPos++;
                    } else {
                        rightDone = true;
                    }

                    if (leftDone && rightDone) {
                        if (fiPos < outBuff.pos + frequency) {
                            _isFinished = true;
                        }
                        break;
                    }
                }
            }
        }
    }
        seconds++;
        if(inBuff.endPos == 0) {
            out.write(outBuff, frequency);
        } else{
            out.write(outBuff,static_cast<unsigned int>(inBuff.endPos-inBuff.pos));
        }
    }
}

void DistortionConverter::printDescription(){
    std::cout << std::setw(10) << std::cout.fill() << "distortion [INITIAL_SECOND] [FINAL_SECOND] [COEFFICIENT]\n";
    std::cout << std::setw(15) << std::cout.fill() << "make distortion effect from INITIAL_SECOND to FINAL_SECOND\n";
    std::cout << std::setw(15) << std::cout.fill() << "COEFFICIENT - level of cutting of wave in percents\n\n";
}