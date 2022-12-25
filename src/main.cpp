#include <iostream>
#include <ostream>
#include <tuple>
#include <sstream>
#include "CSVParser.h"

int main() {
    std::ifstream stream("C:\\pars\\p.txt",std::ios::binary);
    CSVParser<int,std::string,std::string,float> parser(stream,0);
    parser.setDelimiters('\n','.','/');
    for(auto it = parser.begin(); it != parser.end(); it++){
        std::cout << *it;
        std::cout << "\n";
    }
}
