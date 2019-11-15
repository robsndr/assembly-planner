#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "graph_generator.hpp"
// #include "aostar.hpp"
#include "planner.hpp"
// #include "bfs.hpp"


int main(void){

    GraphGenerator graph_gen;

    // graph_gen.insertAnd(2, "a1", 20);
    // graph_gen.insertAnd(3, "a2", 5);
    // graph_gen.insertAnd(4, "a3", 15);

    // graph_gen.insertOr(5, "ABCDEF");
    // graph_gen.insertAnd(6, "a4", 10);

    // graph_gen.insertOr(7, "CDEFGH");
    // graph_gen.insertAnd(8, "a5", 5);

    // graph_gen.insertOr(9, "ABCD");
    // graph_gen.insertAnd(10, "a6", 3);

    // graph_gen.insertOr(11, "CDEF");
    // graph_gen.insertAnd(12, "a7", 5);

    // graph_gen.insertOr(13, "EFGH");
    // graph_gen.insertAnd(14, "a8", 5);


    // graph_gen.insertOr(15, "AB");
    // graph_gen.insertAnd(16, "a9", 5);

    // graph_gen.insertOr(17, "CD");
    // graph_gen.insertAnd(18, "a10", 5);

    // graph_gen.insertOr(19, "EF");
    // graph_gen.insertAnd(20, "a11", 10);

    // graph_gen.insertOr(21, "GH");
    // graph_gen.insertAnd(22, "a12", 10);


    graph_gen.insertAnd(2, "a1", 50);
    graph_gen.insertAnd(3, "a2", 50);
    graph_gen.insertAnd(4, "a3", 50);

    graph_gen.insertOr(5, "ABCDEF");
    graph_gen.insertAnd(6, "a4", 10);

    graph_gen.insertOr(7, "CDEFGH");
    graph_gen.insertAnd(8, "a5", 5);

    graph_gen.insertOr(9, "ABCD");
    graph_gen.insertAnd(10, "a6", 10);

    graph_gen.insertOr(11, "CDEF");
    graph_gen.insertAnd(12, "a7", 5);

    graph_gen.insertOr(13, "EFGH");
    graph_gen.insertAnd(14, "a8", 5);


    graph_gen.insertOr(15, "AB");
    graph_gen.insertAnd(16, "a9", 20);

    graph_gen.insertOr(17, "CD");
    graph_gen.insertAnd(18, "a10", 10);

    graph_gen.insertOr(19, "EF");
    graph_gen.insertAnd(20, "a11", 10);

    graph_gen.insertOr(21, "GH");
    graph_gen.insertAnd(22, "a12", 10);

    graph_gen.insertOr(23, "A");
    graph_gen.insertOr(24, "B");
    graph_gen.insertOr(25, "C");
    graph_gen.insertOr(26, "D");
    graph_gen.insertOr(27, "E");
    graph_gen.insertOr(28, "F");
    graph_gen.insertOr(29, "G");
    graph_gen.insertOr(30, "H");

    Graph<> * graph = graph_gen.generate();
    
    NodeData rdata;
    rdata.name = "ABCDEFGH";
    rdata.type = NodeType::OR;
    rdata.cost = 3;
    Node * root = graph->insertNode(1, rdata);


    // Insert edges
    // ABCDEFGH
    graph->insertEdge(0,1,2);
    graph->insertEdge(0,1,3);
    graph->insertEdge(0,1,4);

    //a1
    graph->insertEdge(0,2,5);
    graph->insertEdge(0,2,21);

    //a2
    graph->insertEdge(0,3,9);
    graph->insertEdge(0,3,13);

    //a3
    graph->insertEdge(0,4,15);
    graph->insertEdge(0,4,7);

    //ABCDEF
    graph->insertEdge(0,5,6);

    //CDEFGH
    graph->insertEdge(0,7,8);

    //a4
    graph->insertEdge(0,6,15);
    graph->insertEdge(0,6,11);

    //a5
    graph->insertEdge(0,8,11);
    graph->insertEdge(0,8,21);

    //ABCD
    graph->insertEdge(0,9,10);

    //CDEF
    graph->insertEdge(0,11,12);

    //EFGH
    graph->insertEdge(0,13,14);

    //a6
    graph->insertEdge(0,10,15);
    graph->insertEdge(0,10,17);

    //a7
    graph->insertEdge(0,12,17);
    graph->insertEdge(0,12,19);

    //a8
    graph->insertEdge(0,14,19);
    graph->insertEdge(0,14,21);

    // AB, CD, EF, GH
    graph->insertEdge(0,15,16);
    graph->insertEdge(0,17,18);
    graph->insertEdge(0,19,20);
    graph->insertEdge(0,21,22);

    //a9 -> A, B
    graph->insertEdge(0,16,23);
    graph->insertEdge(0,16,24);

    //a10 -> C, D
    graph->insertEdge(0,18,25);
    graph->insertEdge(0,18,26);

    //a11 -> E, F
    graph->insertEdge(0,20,27);
    graph->insertEdge(0,20,28);

    //a12 -> G, H
    graph->insertEdge(0,22,29);
    graph->insertEdge(0,22,30);

    std::unordered_map< std::string, std::vector<int> > action_cost_map;
    action_cost_map["a1"].push_back(999);
    action_cost_map["a1"].push_back(999);
    action_cost_map["a1"].push_back(20);

    action_cost_map["a2"].push_back(999);
    action_cost_map["a2"].push_back(999);
    action_cost_map["a2"].push_back(5);

    action_cost_map["a3"].push_back(999);
    action_cost_map["a3"].push_back(999);
    action_cost_map["a3"].push_back(15);

    action_cost_map["a4"].push_back(10);
    action_cost_map["a4"].push_back(10);
    action_cost_map["a4"].push_back(20);

    action_cost_map["a5"].push_back(5);
    action_cost_map["a5"].push_back(5);
    action_cost_map["a5"].push_back(5);

    action_cost_map["a6"].push_back(20);
    action_cost_map["a6"].push_back(10);
    action_cost_map["a6"].push_back(3);

    action_cost_map["a7"].push_back(10);
    action_cost_map["a7"].push_back(5);
    action_cost_map["a7"].push_back(15);

    action_cost_map["a8"].push_back(999);
    action_cost_map["a8"].push_back(5);
    action_cost_map["a8"].push_back(10);

    action_cost_map["a9"].push_back(20);
    action_cost_map["a9"].push_back(20);
    action_cost_map["a9"].push_back(5);

    action_cost_map["a10"].push_back(10);
    action_cost_map["a10"].push_back(10);
    action_cost_map["a10"].push_back(5);

    action_cost_map["a11"].push_back(10);
    action_cost_map["a11"].push_back(10);
    action_cost_map["a11"].push_back(10);

    action_cost_map["a12"].push_back(10);
    action_cost_map["a12"].push_back(10);
    action_cost_map["a12"].push_back(10);

    Planner planner;
    planner(graph, root, action_cost_map);

    // graph->print();

    return 0;
}