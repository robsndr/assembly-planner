#pragma once

#include <string>
#include "graph.hpp"
#include <math.h>

class GraphGenerator
{
public:
    GraphGenerator(Graph<> *);
    ~GraphGenerator();

    std::size_t insertAnd(std::string);
    std::size_t insertOr(std::string);
    bool setRoot(std::string);
    bool insertEdge(std::string, std::string);
    Graph<> *graph_;

private:
    std::vector<Node *> and_;
    std::vector<Node *> or_;
    std::unordered_map<std::string, std::size_t> id_map;
};

GraphGenerator::GraphGenerator(Graph<> *graph)
{
    graph_ = graph;
}

GraphGenerator::~GraphGenerator()
{
}

bool GraphGenerator::setRoot(std::string name)
{
    if (id_map.find(name) == id_map.end())
    {
        std::cout << "Node with provided key does not exist in graph." << std::endl;
        std::cout << "Check the root-key provided in the XML-input file." << std::endl;
        return false;
    }
    else
    {
        graph_->root_ = graph_->getNode(id_map[name]);
        return true;
    }
}

std::size_t GraphGenerator::insertAnd(std::string name)
{
    NodeData data;
    data.name = name;
    data.type = NodeType::AND;
    data.cost = 0;
    data.marked = false;

    Node *inserted_node = graph_->insertNode(data);
    id_map[name] = inserted_node->id_;
    return inserted_node->id_;
}

std::size_t GraphGenerator::insertOr(std::string name)
{
    NodeData data;
    data.name = name;
    data.type = NodeType::AND;
    data.cost = log2(name.length());
    data.marked = false;

    // Node is atomic if it is a leaf node.
    // Subassembly consists only of single part.
    if (name.length() == 1)
        data.atomic = true;
    else
        data.atomic = false;

    Node *inserted_node = graph_->insertNode(data);
    id_map[name] = inserted_node->id_;
    return inserted_node->id_;
}

bool GraphGenerator::insertEdge(std::string start, std::string end)
{
    EdgeData edata;
    edata.cost = 0;

    if ((id_map.find(start) == id_map.end()) || (id_map.find(end) == id_map.end()))
    {
        std::cerr << "GraphGenerator: Could not create edge. Nodes not found." << std::endl;
        return false;
    }
    else
    {
        graph_->insertEdge(edata, id_map[start], id_map[end]);
        return true;
    }
}