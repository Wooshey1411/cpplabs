#include "Params.h"
#include <iostream>
#include "./Exceptions.h"

Params::Params():_countOfParams(0) {}

IFParamsC::IFParamsC(){
    _countOfParams = 2;
    _initial = 0;
    _final = 0;
}

void IFParamsC::setParams(unsigned int pos, std::any arg) {
    switch (pos) {
        case 1:
            _initial = std::any_cast<unsigned int>(arg);
            break;
        case 2:
            _final = std::any_cast<unsigned int>(arg);
            break;
        default:
            throw std::exception();
    }
}

std::any IFParamsC::getParams(unsigned int pos) {
    switch (pos) {
        case 1:
            return std::make_any<unsigned int>(_initial);
        case 2:
            return std::make_any<unsigned int>(_final);
        default:
            throw std::exception();
    }
}

PIPParamsC::PIPParamsC() {
    _countOfParams = 3;
    _initial = 0;

}

void PIPParamsC::setParams(unsigned int pos, std::any arg) {
    switch (pos) {
        case 1:
            _path = std::any_cast<std::string>(arg);
            break;
        case 2:
            _initial = std::any_cast<unsigned int>(arg);
            break;
        case 3:
            pFunction = std::any_cast<void(*)()>(arg);
            break;
        default:
            throw std::exception();
    }
}

std::any PIPParamsC::getParams(unsigned int pos) {
    switch (pos) {
        case 1:
            return std::make_any<std::string>(_path);
        case 2:
            return std::make_any<unsigned int>(_initial);
        case 3:
            return std::make_any<void (*)()>(pFunction);
        default:
            throw std::exception();
    }
}

IFCParamsC::IFCParamsC() {
    _countOfParams = 3;
    _initial = 0;
    _final = 0;
    _coefficient = 0;
}

void IFCParamsC::setParams(unsigned int pos, std::any arg) {
    switch (pos) {
        case 1:
            _initial = std::any_cast<unsigned int>(arg);
            break;
        case 2:
            _final = std::any_cast<unsigned int>(arg);
            break;
        case 3:
            _coefficient = std::any_cast<unsigned int>(arg);
            break;
        default:
            throw std::exception();
    }
}

std::any IFCParamsC::getParams(unsigned int pos) {
    switch (pos) {
        case 1:
            return std::make_any<unsigned int>(_initial);
        case 2:
            return std::make_any<unsigned int>(_final);
        case 3:
            return std::make_any<unsigned int>(_coefficient);
        default:
            throw std::exception();
    }
}