#include "Processor.h"
#include "WAVWriter.h"
#include "WAVReader.h"
#include "BufferPipeline.h"
#include "Converters/ConvertersFactory.h"

void Processor::convert(std::string_view in, std::string_view out, std::vector<std::variant<std::string,unsigned int>> &args) {
    WAVReader reader(in);
    WAVWriter writer(out);
    reader.readHeader();
    writer.writeHeader(reader.getHeader());
    BufferPipeline buff;
    ConvertersFactory factory;
    std::unique_ptr<Converter> converter = factory.createConverter(std::get<std::string>(args[0]));
    args.erase(args.begin());
    while (reader.readByFrequency(&buff)){
        converter->convert(args,&buff);
        writer.writeByFrequency(&buff);
    }
    for (unsigned int i = 0; i < converter->getCountOfParams(); ++i) {
        args.erase(args.begin());
    }
}