#pragma once

#include <string>
#include <math.h>

#include "graph.hpp"

// Graph factory
//  Provides an adaptation layer to model the assembly and-or structure.
//
struct GraphFactory
{
    GraphFactory(Graph<AssemblyData,EdgeData> *);

    std::size_t insertAnd(std::string);
    std::size_t insertOr(std::string);
    bool setRoot(std::string);
    bool insertEdge(std::string, std::string);

    Graph<AssemblyData,EdgeData> *graph;

    std::unordered_map<std::string, std::size_t> id_map;

  private:
    std::vector<Node<AssemblyData> *> and_;
    std::vector<Node<AssemblyData> *> or_;
};

GraphFactory::GraphFactory(Graph<AssemblyData,EdgeData> *graph)
    : graph(graph)
{}

bool GraphFactory::setRoot(std::string name)
{
    // Check if node if given name exists within the graph
    if (id_map.find(name) == id_map.end())
    {
        std::cout << "Node with provided key does not exist in graph." << std::endl
                  << "Check the root-key provided in the XML-input file." << std::endl;
        return false;
    }
    graph->root = graph->getNode(id_map[name]);
    return true;
}

// Insert And-Node into graph.
//  @name: name of the node to create
//  \return: unique id of the node inserted
//
NodeIndex GraphFactory::insertAnd(std::string name)
{
    AssemblyData data;
    data.name = name;
    data.type = NodeType::ACTION;

    auto inserted_node_id = graph->insertNode(data);
    id_map[name] = inserted_node_id;
    return inserted_node_id;
}

// Insert Or-Node into graph.
//  @name: name of the node to create
//  \return: unique id of the node inserted
//
NodeIndex GraphFactory::insertOr(std::string name)
{
    AssemblyData data;
    data.name = name;
    data.type = NodeType::SUBASSEMBLY;

    auto inserted_node_id = graph->insertNode(data);
    id_map[name] = inserted_node_id;
    return inserted_node_id;
}

// Insert edge connecting two nodes with provided names.
//  @start: name of the edge source node.
//  @end: name of the edge destination.
//  \return: boolean indicating if successful.
//
bool GraphFactory::insertEdge(std::string start, std::string end)
{
    // Check if nodes with given names are available inside the graph
    if (id_map.find(start) == id_map.end())
    {
        std::cerr << "GraphFactory: Could not create edge - node "
                    << start << " does not exist." << std::endl;
        return false;
    }
    if (id_map.find(end) == id_map.end())
    {
        std::cerr << "GraphFactory: Could not create edge - node "
                    << start << " does not exist." << std::endl;
        return false;
    }
    graph->insertEdge(EdgeData{.cost = 0}, id_map[start], id_map[end]);
    return true;
}