#include <iostream>
#include <fstream>
#include <string>
#include "Processor.h"
#include "Params.h"

int main() {
    Processor processor;
    PIParams piParams = {"C:\\aboba\\moca.wav",0};
    IFParams ifParams = {0,108};
    IFCParams  ifcParams = {0,100,10};
    processor.convert("C:\\aboba\\kal.wav","C:\\aboba\\out.wav","distortion",&ifcParams);
    return 0;
}
