#include <iostream>
#include <ostream>
#include <tuple>
#include <sstream>
#include "CSVParser.h"

int main() {
    CSVParser<int,std::string,std::string,std::string,int,std::string,int,std::string> parser("C:\\pars\\p.txt",0);
   // parser.setDelimiters('\n',',','\\');
   auto iterator = parser.begin();
   iterator++;
    for(auto it = parser.begin(); it != parser.end(); it++){
        std::cout << *it;
        std::cout << "\n";
    }
}
