#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "graph_generator.hpp"
// #include "aostar.hpp"
#include "planner.hpp"
// #include "bfs.hpp"

#include "dotwriter.hpp"

int main(void){

    GraphGenerator graph_gen;

    std::unordered_map<std::string, std::size_t> id_map;

    // id_map["a1"] = graph_gen.insertAnd("a1", 20);
    // id_map["a2"] = graph_gen.insertAnd("a2", 5);
    // id_map["a3"] = graph_gen.insertAnd("a3", 15);

    // id_map["ABCDEF"] = graph_gen.insertOr("ABCDEF");
    // id_map["a4"]     = graph_gen.insertAnd("a4", 10);

    // id_map["CDEFGH"] = graph_gen.insertOr("CDEFGH");
    // id_map["a5"] = graph_gen.insertAnd("a5", 5);

    // id_map["ABCD"] = graph_gen.insertOr("ABCD");
    // id_map["a6"] = graph_gen.insertAnd("a6", 3);

    // id_map["CDEF"] = graph_gen.insertOr("CDEF");
    // id_map["a7"] = graph_gen.insertAnd("a7", 5);

    // id_map["EFGH"] = graph_gen.insertOr("EFGH");
    // id_map["a8"] = graph_gen.insertAnd("a8", 5);


    // id_map["AB"] = graph_gen.insertOr("AB");
    // id_map["a9"] = graph_gen.insertAnd("a9", 5);

    // id_map["CD"] = graph_gen.insertOr("CD");
    // id_map["a10"] = graph_gen.insertAnd("a10", 5);

    // id_map["EF"] = graph_gen.insertOr("EF");
    // id_map["a11"] = graph_gen.insertAnd("a11", 10);

    // id_map["GH"] = graph_gen.insertOr("GH");
    // id_map["a12"] = graph_gen.insertAnd("a12", 10);



    id_map["ABCDEFGH"] = graph_gen.insertOr("ABCDEFGH");

    id_map["a1"] = graph_gen.insertAnd("a1", 50);
    id_map["a2"] = graph_gen.insertAnd("a2", 50);
    id_map["a3"] = graph_gen.insertAnd("a3", 50);

    id_map["ABCDEF"] = graph_gen.insertOr("ABCDEF");
    id_map["a4"] = graph_gen.insertAnd("a4", 10);

    id_map["CDEFGH"] = graph_gen.insertOr("CDEFGH");
    id_map["a5"] = graph_gen.insertAnd("a5", 5);

    id_map["ABCD"] = graph_gen.insertOr("ABCD");
    id_map["a6"] = graph_gen.insertAnd("a6", 10);

    id_map["CDEF"] = graph_gen.insertOr("CDEF");
    id_map["a7"] = graph_gen.insertAnd("a7", 5);

    id_map["EFGH"] = graph_gen.insertOr("EFGH");
    id_map["a8"] = graph_gen.insertAnd("a8", 5);


    id_map["AB"] = graph_gen.insertOr("AB");
    id_map["a9"] = graph_gen.insertAnd("a9", 20);

    id_map["CD"] = graph_gen.insertOr("CD");
    id_map["a10"] = graph_gen.insertAnd("a10", 10);

    id_map["EF"] = graph_gen.insertOr("EF");
    id_map["a11"] = graph_gen.insertAnd("a11", 10);

    id_map["GH"] = graph_gen.insertOr("GH");
    id_map["a12"] = graph_gen.insertAnd("a12", 10);

    id_map["A"] = graph_gen.insertOr("A");
    id_map["B"] = graph_gen.insertOr("B");
    id_map["C"] = graph_gen.insertOr("C");
    id_map["D"] = graph_gen.insertOr("D");
    id_map["E"] = graph_gen.insertOr("E");
    id_map["F"] = graph_gen.insertOr("F");
    id_map["G"] = graph_gen.insertOr("G");
    id_map["H"] = graph_gen.insertOr("H");
    
    Graph<> * graph = graph_gen.graph;

    // Insert edges
    // ABCDEFGH
    graph->insertEdge(0, id_map["ABCDEFGH"], id_map["a1"]);
    graph->insertEdge(0, id_map["ABCDEFGH"], id_map["a2"]);
    graph->insertEdge(0, id_map["ABCDEFGH"], id_map["a3"]);

    //a1
    graph->insertEdge(0, id_map["a1"], id_map["ABCDEF"]);
    graph->insertEdge(0, id_map["a1"], id_map["GH"]);

    //a2
    graph->insertEdge(0, id_map["a2"], id_map["ABCD"]);
    graph->insertEdge(0, id_map["a2"], id_map["EFGH"]);

    //a3
    graph->insertEdge(0, id_map["a3"], id_map["AB"]);
    graph->insertEdge(0, id_map["a3"], id_map["CDEFGH"]);

    //ABCDEF
    graph->insertEdge(0, id_map["ABCDEF"], id_map["a4"]);

    //CDEFGH
    graph->insertEdge(0, id_map["CDEFGH"], id_map["a5"]);

    //a4
    graph->insertEdge(0, id_map["a4"], id_map["AB"]);
    graph->insertEdge(0, id_map["a4"], id_map["CDEF"]);

    //a5
    graph->insertEdge(0, id_map["a5"], id_map["CDEF"]);
    graph->insertEdge(0, id_map["a5"], id_map["GH"]);

    //ABCD
    graph->insertEdge(0, id_map["ABCD"], id_map["a6"]);

    //CDEF
    graph->insertEdge(0, id_map["CDEF"], id_map["a7"]);

    //EFGH
    graph->insertEdge(0, id_map["EFGH"], id_map["a8"]);

    //a6
    graph->insertEdge(0, id_map["a6"], id_map["AB"]);
    graph->insertEdge(0, id_map["a6"], id_map["CD"]);

    // //a7
    graph->insertEdge(0, id_map["a7"], id_map["CD"]);
    graph->insertEdge(0, id_map["a7"], id_map["EF"]);

    //a8
    graph->insertEdge(0, id_map["a8"], id_map["EF"]);
    graph->insertEdge(0, id_map["a8"], id_map["GH"]);

    // // // AB, CD, EF, GH
    graph->insertEdge(0, id_map["AB"], id_map["a9"]);
    graph->insertEdge(0, id_map["CD"], id_map["a10"]);
    graph->insertEdge(0, id_map["EF"], id_map["a11"]);
    graph->insertEdge(0, id_map["GH"], id_map["a12"]);

    // //a9 -> A, B
    graph->insertEdge(0, id_map["a9"], id_map["A"]);
    graph->insertEdge(0, id_map["a9"], id_map["B"]);

    // //a10 -> C, D
    graph->insertEdge(0, id_map["a10"], id_map["C"]);
    graph->insertEdge(0, id_map["a10"], id_map["D"]);

    // //a11 -> E, F
    graph->insertEdge(0, id_map["a11"], id_map["E"]);
    graph->insertEdge(0, id_map["a11"], id_map["F"]);

    // //a12 -> G, H
    graph->insertEdge(0, id_map["a12"], id_map["G"]);
    graph->insertEdge(0, id_map["a12"], id_map["H"]);


    CostMap action_cost_map(12, 2);

    // action_cost_map.addMapping("a1", "r1", 999);
    action_cost_map.addMapping("a1", "r2", 999);
    action_cost_map.addMapping("a1", "h",  20);

    // action_cost_map.addMapping("a2", "r1", 999);
    action_cost_map.addMapping("a2", "r2", 999);
    action_cost_map.addMapping("a2", "h",  5);

    // action_cost_map.addMapping("a3", "r1", 999);
    action_cost_map.addMapping("a3", "r2", 999);
    action_cost_map.addMapping("a3", "h",  15);

    // action_cost_map.addMapping("a4", "r1", 10);
    action_cost_map.addMapping("a4", "r2", 10);
    action_cost_map.addMapping("a4", "h",  20);

    // action_cost_map.addMapping("a5", "r1", 5);
    action_cost_map.addMapping("a5", "r2", 5);
    action_cost_map.addMapping("a5", "h",  5);

    // action_cost_map.addMapping("a6", "r1", 20);
    action_cost_map.addMapping("a6", "r2", 10);
    action_cost_map.addMapping("a6", "h",  3);


    // action_cost_map.addMapping("a7", "r1", 10);
    action_cost_map.addMapping("a7", "r2", 5);
    action_cost_map.addMapping("a7", "h",  15);


    // action_cost_map.addMapping("a8", "r1", 99);
    action_cost_map.addMapping("a8", "r2", 5);
    action_cost_map.addMapping("a8", "h",  10);


    // action_cost_map.addMapping("a9", "r1", 20);
    action_cost_map.addMapping("a9", "r2", 20);
    action_cost_map.addMapping("a9", "h",  5);


    // action_cost_map.addMapping("a10", "r1", 10);
    action_cost_map.addMapping("a10", "r2", 10);
    action_cost_map.addMapping("a10", "h",  5);


    // action_cost_map.addMapping("a11", "r1", 10);
    action_cost_map.addMapping("a11", "r2", 10);
    action_cost_map.addMapping("a11", "h",  10);


    // action_cost_map.addMapping("a12", "r1", 10);
    action_cost_map.addMapping("a12", "r2", 10);
    action_cost_map.addMapping("a12", "h",  10);

    Node* root = graph->getNode(id_map["ABCDEFGH"]);

    graph->root_ = root;

    Planner planner;
    planner(graph, root, action_cost_map);


    Graph graph_copy(*graph);




    DotWriter dot1("copy_graph.dot");
    graph_copy.print(dot1);

    // graph->appendSubgraph(root, &graph_copy);

    DotWriter dot("origin.dot");
    graph->print(dot);

    return 0;
}