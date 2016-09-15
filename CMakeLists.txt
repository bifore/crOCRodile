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
        src/api/router.c src/api/router.h src/api/crusader/crusader.c src/api/crusader/crusader.h src/api/crusader/socketutils.c src/api/crusader/socketutils.h)

add_executable(OCRodile ${SOURCE_FILES})
