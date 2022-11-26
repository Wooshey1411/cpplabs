#include <iostream>
#include <fstream>
#include <string>
#include "WAVReader.h"
#include "WAVWriter.h"


int main() {
    WAVReader reader("C:\\aboba\\lesnik.wav");
    reader.readHeader();
    reader.printHeader();
    WAVWriter writer("C:\\aboba\\out");
    writer.writeHeader(reader.getHeader());
    return 0;
}
