#include <iostream>
#include <ostream>
#include <tuple>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include "CSVParser.h"

template<class... Args,class V>
auto increaseTuple(std::tuple<Args...> t,V value){
    return std::tuple_cat(t,std::make_tuple(value));
}


template<class V>
void convertToType(const std::string& input, V &val){
    std::istringstream stream(input);
    stream >> val;
    char s;
    stream >> s;
    if (!stream.eof()){
        throw std::runtime_error("error");
    }
}

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

template<std::size_t N, typename T, typename... types>
struct get_Nth_type
{
    using type = typename get_Nth_type<N - 1, types...>::type;
};

template<typename T, typename... types>
struct get_Nth_type<0, T, types...>
{
    using type = T;
};


int main() {
   /* std::tuple<int,char,std::string,std::string,int> tuple{1,'m',"Vor","AD",11};
    // std::get<0>(tuple) = 35;
    //  std::get<1>(tuple) = 'd';

    std::string data = "-2412";
    uint16_t num = 0;
    convertToType<uint16_t>(data,num);
    std::cout << num << "\n";
    std::cout << tuple;
    std::cout << std::endl;
    get_Nth_type<4,int,char,std::string,std::string,int>::type type;

    std::cout << typeid(type).name() << "\n";*/

    std::ifstream stream("C:\\pars\\p.txt");
    CSVParser<std::string,std::string,std::string,std::string> parser(stream,0);
    parser.setDelimiters('\n',';','/');
    for (auto a = parser.begin(); a.operator!=(parser.end()) ; a.operator++()) {
        auto d = a.parseLine();
        for(auto & it : d){
            std::cout << it << " ";
        }
        std::cout << "\n";
    }

}
