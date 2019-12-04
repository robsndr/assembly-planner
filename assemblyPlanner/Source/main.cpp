#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>

#include "planner.hpp"
#include "dotwriter.hpp"
#include "input_reader.hpp"

int main(void){


    InputReader rdr("assembly1.xml");

    auto t1 = std::chrono::high_resolution_clock::now();
    
    Graph<> * assembly;
    CostMap * cost_map;

    std::tie(assembly, cost_map) = rdr.read();

    Planner planner;
    planner(assembly, assembly->root_, *cost_map);


    // Graph graph_copy(*graph);
    // DotWriter dot1("copy_graph.dot");
    // graph_copy.print(dot1);
    // graph.appendSubgraph(root, &graph_copy);

    // DotWriter dot("origin_graph.dot");
    // graph.print(dot);

    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << "Duration: "<< duration << "ms.";

    return 0;
}