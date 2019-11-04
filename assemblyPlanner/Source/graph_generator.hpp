#include <string>
#include "graph.hpp"

class GraphGenerator{
public:
    GraphGenerator(/* args */);
    ~GraphGenerator();

    void insertAnd(std::size_t, std::string, double);
    void insertOr(std::size_t, std::string, double);

    Graph<> * generate();

private:
    std::vector< Node* > and_;
    std::vector< Node* > or_;
    
};

GraphGenerator::GraphGenerator(/* args */)
{
}

GraphGenerator::~GraphGenerator()
{
}

void GraphGenerator::insertAnd(std::size_t id, std::string name, double cost){
    NodeData data;
    data.name = name;
    data.type = NodeType::AND;
    data.cost = cost;
    data.marked = false;
    data.solved = false;

    Node * temp= new Node(id,data);
    and_.push_back(temp);
}

void GraphGenerator::insertOr(std::size_t id, std::string name, double cost){
    NodeData data;
    data.name = name;
    data.type = NodeType::AND;
    data.cost = cost;
    data.marked = false;
    data.solved = false;

    Node * temp= new Node(id,data);
    or_.push_back(temp);
}

Graph<> * GraphGenerator::generate(){
    Graph<> * graph = new Graph();
    graph->insertNodes(and_); 
    graph->insertNodes(or_);    
    return graph;
}
