cmake_minimum_required(VERSION 2.8.3)
project(AssemblyPLanner)

include_directories ("${PROJECT_SOURCE_DIR}/Header") # Search for function declarations in Header/*.hpp
add_subdirectory (Header)                            # Ensures that Header Libarary is built.
set (LIBS ${LIBS} HEADER_LIB)

add_executable(hallowelt Source/main.cpp)
target_link_libraries (hallowelt  ${LIBS})


