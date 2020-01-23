#pragma once

#include <string>
#include <math.h>

#include "graph.hpp"

/* Wrapper/Adapter for the Graph-Class.
    The Graph-Class can be used to model any type of directed/undirected, cyclic/acyclic graph.
    This class provides an adaptation Layer to model AO-Graphs using the Graph-Class.
**/
class GraphGenerator
{
public:

    //Constructor, Destructor
    GraphGenerator(Graph<> *);
    ~GraphGenerator();

    // Insert Nodes. Edges
    std::size_t insertAnd(std::string);
    std::size_t insertOr(std::string);
    bool setRoot(std::string);
    bool insertEdge(std::string, std::string);

    // Pointer to the original graph_ class.
    Graph<> *graph_;

private:
    // Containers
    std::vector<Node *> and_;
    std::vector<Node *> or_;
    std::unordered_map<std::string, std::size_t> id_map;
};

/* Contructor.
    @graph: pointer to the graph where And/Or nodes should be added.
**/
GraphGenerator::GraphGenerator(Graph<> *graph)
{
    graph_ = graph;
}

/* Destructor.
**/
GraphGenerator::~GraphGenerator()
{
}

/* Root Setter.
    @graph: name of the node which should be set as root.
    \return: boolean indication if successful.
**/
bool GraphGenerator::setRoot(std::string name)
{
    // Check if node if given name exists within the graph.
    // Error if not does not exist.
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

/* Insert And-Node into graph.
    @name: name of the node to create
    \return: unique id of the node inserted
**/
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

/* Insert Or-Node into graph.
    @name: name of the node to create
    \return: unique id of the node inserted
**/
std::size_t GraphGenerator::insertOr(std::string name)
{
    NodeData data;
    data.name = name;
    data.type = NodeType::AND;
    data.cost = log2(name.length());
    data.marked = false;

    Node *inserted_node = graph_->insertNode(data);
    id_map[name] = inserted_node->id_;
    return inserted_node->id_;
}

/* Insert Edge connecting two Nodes with provided names.
    @start: name of the edge source node.
    @end: name of the edge destination.
    \return: boolean indicating if successful.
**/
bool GraphGenerator::insertEdge(std::string start, std::string end)
{
    // Data contained within edge
    EdgeData edata;
    edata.cost = 0;

    // Check if nodes with given names are available inside the graph.
    // Error if unavailable.
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