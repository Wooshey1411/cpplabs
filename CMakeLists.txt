cmake_minimum_required(VERSION 3.23)
project(SoundProcessor)

set(CMAKE_CXX_STANDARD 17)

add_executable(SoundProcessor src/main.cpp src/WAVReader.cpp src/WAVWriter.cpp src/BufferPipeline.cpp src/Processor.cpp  src/Converters/ConvertersFactory.cpp src/Converters/Converters.h src/Converters/Converters.cpp src/Params.h)
