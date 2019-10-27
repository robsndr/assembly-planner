cmake_minimum_required(VERSION 2.8.3)
project(AssemblyPLanner)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories ("${PROJECT_SOURCE_DIR}/Lib") # Search for function declarations inLib/*.hpp
add_subdirectory (Lib)                            # Ensures that Libararies are built.
set (LIBS ${LIBS} HEADER_LIB)

add_executable(hallowelt Source/main.cpp)
target_link_libraries (hallowelt  ${LIBS})


