#include "../BufferPipeline.h"
class Converter{
public:
    virtual void convert(void*,BufferPipeline*) = 0;
    virtual ~Converter() = default;
};