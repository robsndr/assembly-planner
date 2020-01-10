cmake_minimum_required(VERSION 2.8.3)
project(AssemblyPLanner)


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.8
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.3)
        message(FATAL_ERROR "GCC version must be at least 8.3!")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # require at least clang 7.0
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
        message(FATAL_ERROR "Clang version must be at least 7.0!")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
endif()



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -DNDEBUG")

include_directories ("${PROJECT_SOURCE_DIR}/Lib") # Search for function declarations inLib/*.hpp
add_subdirectory (Lib)                            # Ensures that Libararies are built.
set (LIBS ${LIBS})

add_executable(planner Source/main.cpp Lib/tinyxml2.cpp)
target_link_libraries (planner  ${LIBS})


