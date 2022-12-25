#include <iostream>
#include <ostream>
#include <tuple>
#include <sstream>
#include "CSVParser.h"

int main() {
    std::ifstream stream("C:\\pars\\p.txt");
    CSVParser<std::string,float> parser(stream,0);
   // parser.setDelimiters('\n',';','/');
    auto a =  parser.begin();
    auto b = parser.begin();
    a++;
    b+=1;
    auto tup = *b;
    std::cout << tup;

}
