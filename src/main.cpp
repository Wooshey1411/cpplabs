#include <iostream>
#include <fstream>
#include <string>
#include "Processor.h"
#include "Params.h"
#include <vector>

int main() {
    Processor processor;
    PIParams piParams = {"C:\\aboba\\lesnik.wav",0};
    IFParams ifParams = {0,60};
    IFCParams  ifcParams = {0,100,40};

    std::vector<std::string> converters;
    std::vector<void*> params;
    params.push_back(&piParams);
    params.push_back(&ifcParams);
    params.push_back(&ifParams);
    converters.push_back("mix");
    converters.push_back("distortion");
    converters.push_back("mute");

    bool permutator = false;
    std::string outStr = "C:\\aboba\\out.wav";
    std::string inStr = "C:\\aboba\\myaso.wav";
    unsigned int pos = 0;
    bool permutated = false;
    for(auto it = converters.begin(); it != converters.end(); it++)
    {
        if(pos == converters.size() - 1){
            if(!permutated){
                processor.convert(inStr,outStr,*it,params[pos]);
            } else {
                processor.convert(permutator ? "tmp2" : "tmp1", outStr, *it, params[pos]);
            }
            break;
        } else{

            if(!permutated){
                processor.convert(inStr,"tmp1",*it,params[pos]);
                permutated = true;
            } else{
                processor.convert(permutator ? "tmp2" : "tmp1", permutator ? "tmp1" : "tmp2",*it,params[pos]);
                permutator = !permutator;
            }
        }
        pos++;
    }
    if(converters.size() == 2){
        remove("tmp1");
    }
    if(converters.size() >=3){
        remove("tmp1");
        remove("tmp2");
    }
   // processor.convert("C:\\aboba\\moca.wav","C:\\aboba\\out.wav","distortion",&ifcParams);
    return 0;
}
