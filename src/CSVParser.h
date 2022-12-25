#include <fstream>
#include <vector>
#include "Exceptions.h"

inline const std::string EMPTY_STRING = "";
inline const char DEFAULT_LINE_DELIMITER = '\n';
inline const char DEFAULT_COLUMN_DELIMITER = ';';
inline const char DEFAULT_SHIELD_CHAR = 0;


template<size_t I,class Ch,class Tr, class... Args>
auto& tp(std::basic_ostream<Ch,Tr>& o,std::tuple<Args...> t){
    o << std::get<I>(t);
    if constexpr (I+1 < sizeof...(Args)){
        o << ", ";
        return tp<I+1>(o,t);
    } else{
        return o;
    }
}

template<class Ch,class Tr, class... Args>
auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t){
    tp<0>(os,t);
}

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
        CSVIterator(std::ifstream& stream, unsigned int line, CSVParser<Args...> &parser):
               _fileLine(EMPTY_STRING),_isEnd(false),_line(line),_in(stream),_parser(parser),_filePos(0) {}

        bool operator==(const CSVIterator& iterator){
            if(this->_isEnd == iterator._isEnd && this->_fileLine == iterator._fileLine && this->_line == iterator._line){
                return true;
            }
            return false;
        }
        bool operator!=(const CSVIterator& iterator){
            return !(*this == iterator);
        }

        CSVIterator& operator+=(const unsigned int num){
            if(this->_line+num >= _parser._countOfLines){
                this->_fileLine = EMPTY_STRING;
                this->_isEnd = true;
                this->_line = _parser._countOfLines;
                return *this;
            }
            _in.clear();
            _in.seekg(0,std::ios::beg);
            this->_line+=num;
            this->_in.seekg(_filePos);
            for (unsigned int i = 0; i < num; ++i) {
                _parser.getLine(this->_in,this->_fileLine);
            }
            _filePos = _in.tellg();
            return *this;
        }
        CSVIterator& operator+(const unsigned int num){
            *this+=num;
            return *this;
        }
        CSVIterator& operator ++(){
            *this+=1;
            return *this;
        }
        CSVIterator operator ++(int){
            CSVIterator iterator(this->_in,this->_line,this->_parser);
            iterator._isEnd = this->_isEnd;
            iterator._fileLine = this->_fileLine;
            iterator._filePos = this->_filePos;

            *this+=1;
            return iterator;
        }

        std::tuple<Args...> operator*(){

            if(this->_line >= this->_parser._countOfLines){
                throw BadPointerException("Pointer cannot be unnamed");
            }

            std::vector<std::string> args = parseLine();
            if(args.size() != sizeof...(Args)){
                throw BadLineException("Count of arguments at line " + std::to_string(_line+1) + " is not " + std::to_string(sizeof...(Args)));
            }

            std::tuple<Args...> tuple;

            try {
                setParam<sizeof...(Args) - 1>(args, tuple);
            }catch(const std::runtime_error& e){
                throw BadLineException("Failed to convert it " + std::string(e.what()) + " column at " + std::to_string(_line+1) + " line");
            }
            return tuple;
        }

    private:
        std::string _fileLine;
        bool _isEnd;
        unsigned int _line;
        std::ifstream& _in;
        CSVParser<Args...>& _parser;
        unsigned long long _filePos;

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
                    if(i == _fileLine.size() - 1){
                        str.push_back(_fileLine[i]);
                    }
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

        template<class V>
        void convertToType(const std::string& input, V &val){
            std::istringstream stream(input);
            stream >> val;
            char s;
            stream >> s;
            if (!stream.eof()){
                throw std::runtime_error("Failed type convert");
            }
        }

        template<std::size_t N>
        std::tuple<Args...> setParam(std::vector<std::string> vector,std::tuple<Args...> &tuple){
            typename std::tuple_element<N, std::tuple<Args...> >::type val;
            try {
                convertToType<typename std::tuple_element<N, std::tuple<Args...>>::type>(vector[N],val);
            }catch(...){
                throw std::runtime_error(std::to_string(N+1));
            }
            std::get<N>(tuple) = val;
            if constexpr (N == 0){
                return tuple;
            } else {
                return setParam<N - 1>(vector, tuple);
            }
        }

    };

public:
    CSVParser(std::ifstream& stream,unsigned int countOfSkips):_countOfSkips(countOfSkips),_countOfLines(0),
    _in(stream),_columnDelimiter(DEFAULT_COLUMN_DELIMITER),_lineDelimiter(DEFAULT_LINE_DELIMITER),_shieldChar(DEFAULT_SHIELD_CHAR) {
        if(!_in.is_open() || _in.bad()){
            throw BadFileException("Stream doesn't open");
        }
        getCountOfLines();
        if(_countOfLines < countOfSkips){
            throw BadFileException("Skipped lines more than lines in file");
        }
    }
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
        _in.clear();
        _in.seekg(0, std::ios::beg);
        getLine(it._in,it._fileLine);
        it._filePos = _in.tellg();
        return it;
    }

    CSVIterator end(){
        CSVIterator it(_in,_countOfLines,*this);
        it._fileLine = EMPTY_STRING;
        it._isEnd = true;
        return it;
    }
};

