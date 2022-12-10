cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

project(SoundProcessor)

set(exe_name ${PROJECT_NAME})

set(test_name test)

set(CMAKE_EXE_LINKER_FLAGS "-static")

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_library(corelib OBJECT src/WAVReader.cpp src/WAVWriter.cpp src/BufferPipeline.cpp src/Processor.cpp  src/Converters/ConvertersFactory.cpp src/Converters/Converters.cpp src/ConfigParser.cpp src/MainUtils.cpp src/Converters/StreamLinker.cpp)

add_executable(${exe_name} src/main.cpp)
target_link_libraries(${exe_name} corelib)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(${test_name} src/tests/Tests.cpp)
target_link_libraries(${test_name} gtest_main)
target_link_libraries(${test_name} corelib)