cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial)

find_package(Boost REQUIRED COMPONENTS system)
FIND_PACKAGE(libmongocxx REQUIRED)
FIND_PACKAGE(libbsoncxx REQUIRED)

INCLUDE_DIRECTORIES(${LIBMONGOCXX_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${LIBBSONCXX_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("/usr/local/include/mongocxx/v_noabi/mongocxx")
INCLUDE_DIRECTORIES("/usr/local/include/bsoncxx/v_noabi/bsoncxx")
INCLUDE_DIRECTORIES("/usr/local/include/libmongoc-1.0")
INCLUDE_DIRECTORIES("/usr/local/include/libbson-1.0")

# add the executable
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -DNDEBUG -pthread -j4")

# add the uWebSockets module 
include_directories ("${PROJECT_SOURCE_DIR}/../Lib/websocketpp")

add_executable(testing franka_test.cpp)
target_link_libraries (testing ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES}  ${Boost_LIBRARIES})