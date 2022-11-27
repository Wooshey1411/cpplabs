cmake_minimum_required(VERSION 3.20)
project(SoundProcessor)

set(CMAKE_CXX_STANDARD 17)

add_executable(SoundProcessor src/main.cpp src/WAVReader.cpp src/WAVWriter.cpp src/BufferPipeline.cpp src/Processor.cpp  src/Converters/ConvertersFactory.cpp src/Converters/Converters.cpp src/ConfigParser.cpp src/Exceptions.cpp)

set(CMAKE_EXE_LINKER_FLAGS "-static")

set(exe_name ${PROJECT_NAME})

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()