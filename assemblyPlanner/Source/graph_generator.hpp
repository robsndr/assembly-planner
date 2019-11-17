#include <string>
#include "graph.hpp"
#include <math.h>      

class GraphGenerator{
public:
    GraphGenerator(/* args */);
    ~GraphGenerator();

    std::size_t insertAnd(std::string, double);
    std::size_t insertOr(std::string);
    Graph<> * graph;

private:
    std::vector< Node* > and_;
    std::vector< Node* > or_;
    
};

GraphGenerator::GraphGenerator(/* args */){
    graph = new Graph();
}

GraphGenerator::~GraphGenerator()
{
}

std::size_t GraphGenerator::insertAnd(std::string name, double cost){
    NodeData data;
    data.name = name;
    data.type = NodeType::AND;
    data.cost = cost;
    data.marked = false;
    data.solved = false;

    Node * inserted_node = graph->insertNode(data);
    return inserted_node->id_;
}

std::size_t GraphGenerator::insertOr(std::string name){
    NodeData data;
    data.name = name;
    data.type = NodeType::AND;
    data.cost = log2(name.length());
    data.marked = false;
    data.solved = false;

    Node * inserted_node = graph->insertNode(data);
    return inserted_node->id_;
}
