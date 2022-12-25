#include <iostream>
#include <ostream>
#include <tuple>
#include <sstream>
#include "CSVParser.h"

int main() {
    CSVParser<int> parser("C:\\pars\\p.txt",0);
   // parser.setDelimiters('\n',',','\\');
    for(auto it = parser.begin(); it != parser.end(); it++){
        std::cout << *it;
        std::cout << "\n";
    }
}
