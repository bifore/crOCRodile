cmake_minimum_required(VERSION 3.6)
project(OCRodile)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/main.c
    src/main.h
    .gitignore
    AUTHORS
    Makefile
    README)

add_executable(OCRodile ${SOURCE_FILES})