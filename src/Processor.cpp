#include "Processor.h"
#include "Writer/WAVWriter.h"
#include "Reader/WAVReader.h"
#include "Converters/ConvertersFactory.h"

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
    std::string temp1Path = files[0] + '1';
    std::string temp2Path = files[0] + '2';
    bool tmpUsed = false;
    bool isFirst = true;
    bool permutator = false;
    while(!args.empty()) {
        if (isFirst) {
            auto reader = std::make_unique<WAVReader>(files[1]);
            WAVWriter writer(temp1Path);
            isFirst = false;
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
        } else{
            tmpUsed = true;
            auto reader = std::make_unique<WAVReader>(!permutator ? temp1Path : temp2Path);
            WAVWriter writer(!permutator ? temp2Path : temp1Path);
            permutator = !permutator;

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
    }
    if(!tmpUsed){
        rename((temp1Path).c_str(),files[0].c_str());
    } else{
        if(!permutator){
            rename((temp1Path).c_str(),files[0].c_str());
            remove(temp2Path.c_str());
        } else{
            rename((temp2Path).c_str(),files[0].c_str());
            remove(temp1Path.c_str());
        }
    }
}