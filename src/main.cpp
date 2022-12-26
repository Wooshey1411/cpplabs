#include <iostream>
#include <ostream>
#include <tuple>
#include <sstream>
#include "CSVParser.h"

int main() {
  // CSVParser<int,std::string,std::string,std::string,int,std::string,int,std::string> parser("C:\\pars\\p.txt",0);
    CSVParser<std::string,std::string,std::string> parser("C:\\pars\\p2.txt",0);
    parser.setDelimiters('\n','.','"');
 //  auto iterator = parser.begin();
//   iterator++;
//   iterator+5;
    for(auto it = parser.begin(); it != parser.end(); it++){
        std::cout << *it;
        std::cout << "\n";
    }
  // std::tuple<> tuple;
  // std::cout << tuple;
 //   std::cout << *iterator;
}
