#include "CSVParser.h"
#include <fstream>
#include <iostream>


template<class... Args>
CSVParser<Args>::CSVParser(std::ifstream& stream, unsigned int countOfSkips):_countOfSkips(countOfSkips),_countOfLines(0),
_in(stream),_columnDelimiter(';'),_lineDelimiter('\n'),_shieldChar(0) {getCountOfLines();}

template<class Ch, class Tr, class Al>
void CSVParser::getLine(std::basic_ifstream<Ch, Tr> &i, std::basic_string<Ch, Tr, Al> &str) {
    str.clear();
    char c;
    while(i.get(c)){
        if(c == _lineDelimiter) {
            break;
        }
        str.push_back(c);
    }
}

void CSVParser::getCountOfLines() {
    if (_countOfLines == 0) {
        _in.clear();
        _in.seekg(0, std::ios::beg);

        std::string str;
        _countOfLines = 0;
        while (!_in.eof()) {
            getLine(_in, str);
            _countOfLines++;
        }
        _in.clear();
        _in.seekg(0,std::ios::beg);
    }
}

void CSVParser::setDelimiters(const char lineDelimiter, const char columnDelimiter, const char shieldChar){
    _lineDelimiter = lineDelimiter;
    _columnDelimiter = columnDelimiter;
    _shieldChar = shieldChar;
}


void CSVParser::printLine() {
    std::string str;
    getLine(_in,str);
    std::cout << str << "\n";
}

CSVParser::CSVIterator::CSVIterator(std::ifstream& stream, unsigned int pos, CSVParser<Args...> &parser:
_pos(pos),_parser(parser),_in(stream),_isEnd(false) {}