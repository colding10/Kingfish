cmake_minimum_required(VERSION 3.17)

project(Kingfish
    VERSION 1.2.0
    DESCRIPTION "A UCI chess engine"
    LANGUAGES CXX
)

add_executable(kingfish
    src/polyglot.cpp,
    src/searcher.cpp
    src/uci.cpp)