#include "ConvertersFactory.h"
#include <string>
template<class T>
std::shared_ptr<Converter> createCnv(){
    auto a = std::make_shared<T>();
    return a;
}

ConvertersFactory::ConvertersFactory() {
    _converters["mute"] = createCnv<MuteConverter>;
    _converters["mix"] = createCnv<MixConverter>;
    _converters["distortion"] = createCnv<DistortionConverter>;
}

std::shared_ptr<Converter> ConvertersFactory::createConverter(std::string_view name) {
    std::map<std::string, std::shared_ptr<Converter> (*)()>::iterator it;
    it = _converters.find(std::string(name));
    if(it != _converters.end()){
        return it->second();
    }
    throw std::runtime_error("Fabric: Converter with such name doesn't exist");
}