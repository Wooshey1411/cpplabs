#include "Processor.h"
#include "Writer/WAVWriter.h"
#include "Reader/WAVReader.h"
#include "BufferPipeline.h"
#include "Converters/ConvertersFactory.h"

void Processor::convert(std::string_view in, std::string_view out, std::vector<std::variant<std::string,unsigned int>> &args) {
    WAVReader reader(in);
    WAVWriter writer(out);
    reader.readHeader();
    writer.writeHeader(reader.getHeader());
    std::vector<std::unique_ptr<Reader>> streams;
    streams.push_back(std::make_unique<Reader>(in));
    ConvertersFactory factory;
    std::unique_ptr<Converter> converter = factory.createConverter(std::get<std::string>(args[0]));

    args.erase(args.begin());
    converter->convert(args,streams,out,reader.getHeader()->headerMain.samplesPerSec);
    for (unsigned int i = 0; i < converter->getCountOfParams(); ++i) {
        args.erase(args.begin());
    }
}