cmake_minimum_required(VERSION 2.8.3)
project(assembly_planner)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(catkin REQUIRED COMPONENTS
  roscpp
)

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES planner_lib
)

include_directories(include
    ${catkin_INCLUDE_DIRS}
)

add_library(planner_lib
            src/andorgraph_node.cpp
            src/assembly_controller.cpp
            src/a_star.cpp
            src/data_Assembly.cpp
            src/math_functions.cpp
            src/objects.cpp
            src/planner.cpp
            src/thread_Agent.cpp
            src/test.cpp
            src/memory.cpp
)

add_executable(assembly_planner src/main.cpp)

target_link_libraries(assembly_planner planner_lib
    ${catkin_LIBRARIES}
)
