CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
PROJECT(AssemblyPlanner)


IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 7.0
    IF (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
        MESSAGE(FATAL_ERROR "GCC version must be at least 7.0!")
    ENDIF()
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # require at least clang 6.0
    IF (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
        MESSAGE(FATAL_ERROR "Clang version must be at least 6.0!")
    ENDIF()
ELSE()
    MESSAGE(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
ENDIF()

FIND_PACKAGE(Boost REQUIRED COMPONENTS system)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -DNDEBUG -pthread")

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/lib/tinyxml2") # Search for function declarations inLib/*.hpp
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/lib/argparse/include") # Search for function declarations inLib/*.hpp
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/lib/websocketpp")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/lib/json/single_include")


ADD_EXECUTABLE(planner src/main.cpp lib/tinyxml2/tinyxml2.cpp)
TARGET_LINK_LIBRARIES(planner ${LIBS})
