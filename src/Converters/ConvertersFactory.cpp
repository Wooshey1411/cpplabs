#include "ConvertersFactory.h"
#include <string>

template<class T>
std::unique_ptr<Converter> createCnv(){
    auto a = std::make_unique<T>();
    return a;
}

ConvertersFactory::ConvertersFactory() {
    _converters["mute"] = createCnv<MuteConverter>;
    _converters["mix"] = createCnv<MixConverter>;
    _converters["bassBoosted"] = createCnv<BassBoostedConverter>;
    _converters["distortion"] = createCnv<DistortionConverter>;
}

std::unique_ptr<Converter> ConvertersFactory::createConverter(std::string_view name) {

    auto it = _converters.find(std::string(name));
    if(it != _converters.end()){
        return std::move(it->second());
    }
    throw std::runtime_error("Fabric: Converter with such name doesn't exist");
}

void ConvertersFactory::printDescriptions() {
    for(auto & _converter : _converters){
        _converter.second()->printDescription();
    }
}