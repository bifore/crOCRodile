cmake_minimum_required(VERSION 3.6)
project(OCRodile)

set(CMAKE_C_FLAGS "-Wall -Wextra -std=c99 -g")

set(SOURCE_FILES
        src/main.c
        src/io/bmpReader.c
        src/utils/error.c
        src/utils/lkList.c
        src/utils/vector.c
        src/utils/string.c
        )

add_executable(OCRodile ${SOURCE_FILES})