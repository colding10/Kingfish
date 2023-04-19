cmake_minimum_required(VERSION 3.26)
project(kingfish)

# #
# kingfish version numbers
# #
set(KINGFISH_VERSION_MAJOR "1")
set(KINGFISH_VERSION_MINOR "2")
set(KINGFISH_VERSION_PATCH "0")

message("Kingfish version is set to ${KINGFISH_VERSION_MAJOR}.${KINGFISH_VERSION_MINOR}.${KINGFISH_VERSION_PATCH}")

# #
# Executable name
# #
set(KINGFISH_OUT_NAME "kingfish_${KINGFISH_VERSION_MAJOR}.${KINGFISH_VERSION_MINOR}.${KINGFISH_VERSION_PATCH}")

# #
# Project settings
# #
set(CMAKE_CXX_STANDARD 20)

# #
# Source code
# #
add_library(kingfish STATIC
    src/kingfish/types.cpp
    src/kingfish/bitboard.cpp
    src/kingfish/chessgame.cpp
    src/kingfish/endgame.cpp
    src/kingfish/piece.cpp
    src/kingfish/position.cpp
    src/kingfish/zobrist.cpp
    src/kingfish/square.cpp
    src/kingfish/move.cpp
    src/kingfish/perft.cpp
    src/kingfish/kingfishchess.cpp
    src/kingfish/debug.cpp
    src/kingfish/posutils.cpp
    src/kingfish/strutils.cpp
    src/kingfish/ai/classiceval/hotmap.cpp
    src/kingfish/ai/timemanager.cpp
    src/kingfish/ai/classiceval/classicevaluator.cpp
    src/kingfish/ai/search.cpp
    src/kingfish/ai/transpositiontable.cpp
    src/kingfish/ai/aimovefactory.cpp
    src/kingfish/ai/classiceval/aibitboards.cpp
    src/kingfish/ai/quiescevaluator.cpp
    src/kingfish/utils.cpp
    src/kingfish/endgame.cpp
    src/kingfish/endgame.h
    src/kingfish/openingbook.cpp
    src/kingfish/openingbook.h)

# add_executable(kingfishcli
# src/kingfishcli/main.cpp
# src/kingfishcli/uci.cpp
# src/kingfishcli/uci.h)

# add_executable(kingfishtest
# src/kingfishtest/main.cpp
# src/kingfishtest/testlist.cpp)

# #
# Compilation settings
# #
set_target_properties(kingfish PROPERTIES OUTPUT_NAME ${KINGFISH_OUT_NAME})

# #
# Add external dependencies
# #
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC requires some flags in order for threads and <filesystem> to work.
    set(CMAKE_CXX_FLAGS "-pthread -static-libstdc++ -static-libgcc")
    target_link_libraries(kingfish PUBLIC stdc++fs)
else()
    target_link_libraries(kingfish PUBLIC nlohmann_json)
endif()

# target_link_libraries(kingfishcli PRIVATE kingfish)
# target_link_libraries(kingfishtest PRIVATE kingfish)
