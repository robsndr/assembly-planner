#pragma once

#include <string>
#include "graph.hpp"
#include <math.h>      

class GraphGenerator{
public:
    GraphGenerator(Graph<> *);
    ~GraphGenerator();

    std::size_t insertAnd(std::string);
    std::size_t insertOr(std::string);
    Graph<> * graph_;

private:
    std::vector< Node* > and_;
    std::vector< Node* > or_;
    
};

GraphGenerator::GraphGenerator(Graph<> * graph){
    graph_ =graph;
}

GraphGenerator::~GraphGenerator()
{
}

std::size_t GraphGenerator::insertAnd(std::string name){
    NodeData data;
    data.name = name;
    data.type = NodeType::AND;
    data.cost = 0;
    data.marked = false;

    Node * inserted_node = graph_->insertNode(data);
    return inserted_node->id_;
}

std::size_t GraphGenerator::insertOr(std::string name){
    NodeData data;
    data.name = name;
    data.type = NodeType::AND;
    data.cost = log2(name.length());
    data.marked = false;

    Node * inserted_node = graph_->insertNode(data);
    return inserted_node->id_;
}
