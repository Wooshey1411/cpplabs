#include "Readonly.h"

Readonly::Readonly(unsigned short val)
:value(val){}

unsigned short Readonly::getInstance() {
    return value;
}