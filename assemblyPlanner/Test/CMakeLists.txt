cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial)

FIND_PACKAGE(libmongocxx REQUIRED)
FIND_PACKAGE(libbsoncxx REQUIRED)
INCLUDE_DIRECTORIES(${LIBMONGOCXX_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${LIBBSONCXX_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("/usr/local/include/mongocxx/v_noabi/mongocxx")
INCLUDE_DIRECTORIES("/usr/local/include/bsoncxx/v_noabi/bsoncxx")
INCLUDE_DIRECTORIES("/usr/local/include/libmongoc-1.0")
INCLUDE_DIRECTORIES("/usr/local/include/libbson-1.0")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -DNDEBUG")

# add the executable

# add the uWebSockets module 
include_directories ("${PROJECT_SOURCE_DIR}/../Lib/websocketcpp")

add_executable(testing ws_test.cpp)
target_link_libraries (testing ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES})