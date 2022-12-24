#include <fstream>


template<class... Args>
class CSVParser {
public:
    CSVParser(std::ifstream&,unsigned int);
    void setDelimiters(char,char, char);
    void printLine();

private:
    unsigned int _countOfSkips;
    unsigned int _countOfLines;
    std::ifstream& _in;
    char _columnDelimiter;
    char _lineDelimiter;
    char _shieldChar;
    template<class Ch, class Tr, class Al>
    void getLine(std::basic_ifstream<Ch,Tr> &i, std::basic_string<Ch,Tr,Al> &str);
    void getCountOfLines();


    class CSVIterator {
    public:
        CSVIterator(std::ifstream&, unsigned int, CSVParser<Args...>&);
    private:
        std::string _fileLine;
        bool _isEnd;
        unsigned int _pos;
        std::ifstream& _in;
        CSVParser<Args...>& _parser;
        friend class CSVParser;
    };



};

