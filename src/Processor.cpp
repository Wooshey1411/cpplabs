#include "Processor.h"
#include "WAVWriter.h"
#include "WAVReader.h"
#include "Params/Params.h"
#include "BufferPipeline.h"
#include "Converters/ConvertersFactory.h"

void Processor::convert(std::string_view in, std::string_view out, std::string_view name,  std::shared_ptr<Params> params) {
    WAVReader reader(in);
    WAVWriter writer(out);
    reader.readHeader();
    writer.writeHeader(reader.getHeader());
    BufferPipeline buff;
    ConvertersFactory factory;
    std::unique_ptr<Converter> converter = factory.createConverter(name);
    while (reader.readSecond(&buff)){
        converter->convert(params,&buff);
        writer.writeSecond(&buff);
    }
}
