#include <iostream>
#include <fstream>
#include <string>
#include "Processor.h"
#include "Params.h"

int main() {
    Processor processor;
    PIParams piParams = {"C:\\aboba\\moca.wav",0};
    IFParams ifParams = {90,105};
    IFCParams ifcParams = {0,300,10};

    processor.convert("C:\\aboba\\lesnik.wav","C:\\aboba\\out.wav","distortion",&ifcParams);
    return 0;
}
