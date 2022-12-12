#include "Processor.h"
#include "Writer/WAVWriter.h"
#include "Reader/WAVReader.h"
#include "Converters/ConvertersFactory.h"

inline const std::string NO_FILE = "THISFILEDOESNTEXIST.NOTEXIST";

Processor::Processor():_outPath(NO_FILE) {}

void makeConvert(std::vector<std::unique_ptr<Reader>> &streams,std::vector<std::variant<std::string,unsigned int>> &args,ConvertersFactory &factory, std::string& rPath, std::string& wPath){
    auto reader = std::make_unique<WAVReader>(rPath);
    WAVWriter writer(wPath);
    reader->readHeader();
    writer.writeHeader(reader->getHeader());
    unsigned int freq = reader->getHeader()->headerMain.samplesPerSec;
    streams[0] = std::move(reader);

    std::unique_ptr<Converter> converter = factory.createConverter(std::get<std::string>(args[0]));

    args.erase(args.begin());
    converter->convert(args,streams,writer,freq);

    for (unsigned int i = 0; i < converter->getCountOfParams(); ++i) {
        args.erase(args.begin());
    }
}

void Processor::convert(std::vector<std::string> &files,std::vector<std::variant<std::string,unsigned int>> &args) {
    std::vector<std::unique_ptr<Reader>> streams;
    auto it = files.begin();
    it++;
    for(; it != files.end(); it++){
        auto p = std::make_unique<WAVReader>(*it);
        p->readHeader();
        streams.push_back(std::move(p));
    }
    ConvertersFactory factory;
    _outPath = files[0];
    std::string temp1Path = files[0] + ".tmp1";
    std::string temp2Path = files[0] + ".tmp2";
    bool tmpUsed = false;
    bool isFirst = true;
    bool permutator = false;
    while(!args.empty()) {
        if (isFirst) {
            makeConvert(streams,args,factory,files[1],temp1Path);
            isFirst = false;
        } else{
            if(permutator){
                makeConvert(streams,args,factory,temp2Path,temp1Path);
            } else{
                makeConvert(streams,args,factory,temp1Path,temp2Path);
            }
            permutator = !permutator;
            tmpUsed = true;
        }
    }
    remove(files[0].c_str());
    if(!tmpUsed){
        rename((temp1Path).c_str(),files[0].c_str());
    } else{
        if(!permutator){
            rename((temp1Path).c_str(),files[0].c_str());
        } else{
            rename((temp2Path).c_str(),files[0].c_str());
        }
    }
}

Processor::~Processor() {
    if(_outPath != NO_FILE) {
        remove((_outPath + ".tmp1").c_str());
        remove((_outPath + ".tmp2").c_str());
    }
}