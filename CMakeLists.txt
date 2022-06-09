cmake_minimum_required(VERSION 3.20)
project(prat6)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
add_executable(prat6 main.cpp bubble.cpp sort_meters.cpp cocktail_shaker.cpp comb.cpp selection.cpp insertion.cpp
        gnome.cpp shell.cpp quick.cpp tree.cpp merge.cpp heap.cpp radix.cpp kibir.cpp)

add_library(blab STATIC IMPORTED)
set_target_properties(blab PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libblab.a)

target_link_libraries(prat6 blab)
