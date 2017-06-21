cmake_minimum_required(VERSION 3.7)
project(HW3)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c SPArrayList.c SPFIARGame.c SPFIARParser.c SPMainAux.c SPMiniMax.c SPMiniMaxNode.c)
add_executable(HW3 ${SOURCE_FILES})