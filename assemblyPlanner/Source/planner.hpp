#pragma once

#include <iostream>
#include <unordered_map>
#include "dotwriter.hpp"
#include "astar.hpp"

/* Planner Class. 
    Used as a Top_Level supervisor for the planning process.
    Instantiates the AStarSearch, NodeExpander and search_graph.
**/
class Planner
{
public:
    // Start Planning
    std::vector<std::vector<std::tuple<std::string, std::string, double>>>
                                                    operator()(Graph<> *, Node *, Config *);

private:
    // Container used to track the resulting optimal assembly sequence.
    std::vector<std::vector<std::tuple<std::string, std::string, double>>> assembly_plan_;
    Graph<> *search_graph;
};

/* Start Plannning.
    @graph: pointer to the original A/O graph obtained from the InputReader
    @root: pointer to the node the search should start at.
    @config: configuration contianing the cost_map and reachability_map.
    \return: vector containing the assembly plan
**/
std::vector< std::vector< std::tuple< std::string, std::string, double >>> 
Planner::operator()(Graph<> *graph, Node *root, Config *config)
{
    // Create a new Graph.
    // It is a different Graph the the one passed as a function parameter.
    // This one is the graph of Hypernodes used later for the A* search.
    search_graph = new Graph<>();
    Node *new_root = search_graph->insertNode(root->data_);

    // Set the subassemblies and actions of the first supernode.
    // The actions correspond to all possible moves we can take in the first supernode.
    new_root->data_.subassemblies[root->data_.name] = root;
    for (auto &x : root->getSuccessorNodes())
    {
        new_root->data_.actions[x->data_.name] = x;
    }

    // Create the NodeExpander and pass it to the AStarSearch.
    // The AStarSearch uses the received Expander later during the search.
    // If a different expansion-behavior is desired, just modify the exapnder,
    // obeying to the interface used by the AStarSearch.
    NodeExpander *expander = new NodeExpander(search_graph, config);

    // AStarSearch algorithm
    AStarSearch astar;
    Node *result = astar.search(search_graph, new_root, expander);

    // Container used to represent the found agent-action assignement and its cost in a current step.
    std::vector<std::tuple<std::string, std::string, double>> optimum;

    // Container representing the sequence of all agent-actions for the complete solution.
    assembly_plan_.clear();

    // Backtrack the found optimum assembly-sequence.
    double cost = 0;
    while (result->hasPredecessor())
    {
        for (auto &i : result->getPredecessors().front()->data_.agent_actions_)
        {
            std::cout << "Action: " << i.first << " Agent: " << i.second << "    ";
            cost += config->costs_->map_[i.first][i.second];
            optimum.push_back(std::make_tuple(i.first, i.second, cost));
        }
        assembly_plan_.push_back(optimum);
        optimum.clear();
        std::cout << std::endl;
        result = result->getPredecessorNodes().front();
    }

    std::cout << "Cost: " << cost << std::endl;

    delete search_graph;
    delete expander;

    return assembly_plan_;
}
