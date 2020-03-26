CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)
PROJECT(AssemblyPLanner)


IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.8
    IF (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.3)
        MESSAGE(FATAL_ERROR "GCC version must be at least 8.3!")
    ENDIF()
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # require at least clang 7.0
    IF (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
        MESSAGE(FATAL_ERROR "Clang version must be at least 7.0!")
    ENDIF()
ELSE()
    MESSAGE(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
ENDIF()

FIND_PACKAGE(Boost REQUIRED COMPONENTS system)
FIND_PACKAGE(libmongocxx REQUIRED)
FIND_PACKAGE(libbsoncxx REQUIRED)

INCLUDE_DIRECTORIES(${LIBMONGOCXX_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${LIBBSONCXX_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("/usr/local/include/mongocxx/v_noabi/mongocxx")
INCLUDE_DIRECTORIES("/usr/local/include/bsoncxx/v_noabi/bsoncxx")
INCLUDE_DIRECTORIES("/usr/local/include/libmongoc-1.0")
INCLUDE_DIRECTORIES("/usr/local/include/libbson-1.0")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -DNDEBUG -pthread")

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Lib/tinyxml2") # Search for function declarations inLib/*.hpp
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Lib/argparse/include") # Search for function declarations inLib/*.hpp
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Lib/websocketpp")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Lib/json/single_include")


ADD_EXECUTABLE(planner Source/main.cpp Lib/tinyxml2/tinyxml2.cpp)
TARGET_LINK_LIBRARIES(planner  ${LIBS} ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES} ${Boost_LIBRARIES})


