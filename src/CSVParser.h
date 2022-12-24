#include <fstream>
#include <vector>

inline const std::string EMPTY_STRING = "";

template<class... Args>
class CSVParser {
private:
    unsigned int _countOfSkips;
    unsigned int _countOfLines;
    std::ifstream& _in;
    char _columnDelimiter;
    char _lineDelimiter;
    char _shieldChar;

    template<class Ch, class Tr, class Al>
    void getLine(std::basic_ifstream<Ch, Tr> &i, std::basic_string<Ch, Tr, Al> &str) {
        str.clear();
        char c;
        while(i.get(c)){
            if(c == _lineDelimiter) {
                break;
            }
            str.push_back(c);
        }
    }
    void getCountOfLines() {
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



    class CSVIterator {
    friend class CSVParser;
    public:
        CSVIterator(std::ifstream& stream, unsigned int pos, CSVParser<Args...> &parser):
               _fileLine(EMPTY_STRING), _isEnd(false),_pos(pos),_in(stream),_parser(parser) {}
        std::vector<std::string> parseLine() {
            if(_isEnd || _fileLine == EMPTY_STRING){
                throw std::runtime_error("No data");
            }

            if(_fileLine[0] == _parser._columnDelimiter){
                throw std::runtime_error("Bad line");
            }

            std::string str = "";
            bool isEnded = false;
            std::vector<std::string> strings;
            for(unsigned int i = 0; i < _fileLine.length();i++){
                isEnded = false;
                if((_fileLine[i] == _parser._columnDelimiter && _fileLine[i-1] == _parser._shieldChar) || _fileLine[i] == _parser._shieldChar){
                    continue;
                }
                if(_fileLine[i] == _parser._columnDelimiter || i == _fileLine.size()-1){
                    strings.push_back(str);
                    str.clear();
                    isEnded = true;
                    continue;
                }
                str.push_back(_fileLine[i]);
            }

            if(!isEnded){
                throw std::runtime_error("Bad line");
            }

            if(strings.size() != sizeof...(Args)){
                throw std::runtime_error("Bad count of columns");
            }
            return strings;
        }

        bool operator==(const CSVIterator& iterator){
            if(this->_isEnd == iterator._isEnd && this->_fileLine == iterator._fileLine && this->_pos == iterator._pos){
                return true;
            }
            return false;
        }

        bool operator!=(const CSVIterator& iterator){
            return !(*this == iterator);
        }

        CSVIterator operator ++(){
            if(this->_pos == _parser._countOfLines-1){
                this->_fileLine = EMPTY_STRING;
                this->_isEnd = true;
                this->_pos = _parser._countOfLines;
                return *this;
            }
            this->_pos++;
            _parser.getLine(_parser._in,this->_fileLine);
            return *this;
        }

    private:
        std::string _fileLine;
        bool _isEnd;
        unsigned int _pos;
        std::ifstream& _in;
        CSVParser<Args...>& _parser;
    };

public:
    CSVParser(std::ifstream& stream,unsigned int countOfSkips):_countOfSkips(countOfSkips),_countOfLines(0),
    _in(stream),_columnDelimiter(';'),_lineDelimiter('\n'),_shieldChar(0) {getCountOfLines();}
    void setDelimiters(const char lineDelimiter, const char columnDelimiter, const char shieldChar){
        _lineDelimiter = lineDelimiter;
        _columnDelimiter = columnDelimiter;
        _shieldChar = shieldChar;
    }
    void printLine() {
        std::string str;
        getLine(_in,str);
        std::cout << str << "\n";
    }

    CSVIterator begin(){
        CSVIterator it(_in,_countOfSkips,*this);
        getLine(_in,it._fileLine);
        return it;
    }

    CSVIterator end(){
        CSVIterator it(_in,_countOfLines,*this);
        it._fileLine = EMPTY_STRING;
        it._isEnd = true;
        return it;
    }



};

