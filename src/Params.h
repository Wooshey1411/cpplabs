#include <string>
typedef struct INITIAL_FINAL_PARAMS{
    unsigned int initial;
    unsigned int final;
} IFParams;

typedef struct PATH_INITIAL_PARAMS{
    std::string path;
    unsigned int initial;
}PIParams;

typedef struct INITIAL_FINAL_COEFFICIENT{
    unsigned int initial;
    unsigned int final;
    unsigned int coefficient;
} IFCParams;