#pragma once

#include <iostream>
#include <unordered_map>
#include "dotwriter.hpp"
#include "astar.hpp"
#include "task.hpp"

// Planner - used as a top-level supervisor for the planning process
struct Planner
{
    Planner() = default;
    Planner(const Planner&) = default;
    // Start Planning
    std::vector< std::vector<Task*>> operator()(Graph<AssemblyData,EdgeData>, config::Configuration& );
  private:
    // Container tracking the optimal assembly sequence
    std::vector<std::vector<Task*>> assembly_plan_;
};

// Start planning
//   @graph:  pointer to the original A/O graph obtained from the InputReader
//   @config: configuration contianing the cost_map and reachability_map
//   \return: vector containing the assembly plan
//
std::vector< std::vector<Task*>> 
Planner::operator()(Graph<AssemblyData,EdgeData> graph, config::Configuration& config)
{
    // Create a new Graph.
    // It is a different graph the the one passed as a function parameter.
    // This one is the graph of hypernodes used later for the A* search.
    Graph<SearchData,EdgeData> search_graph;
    auto new_root_id = search_graph.insertNode(SearchData());
    Node<SearchData> *new_root = search_graph.getNode(new_root_id);

    // Set the subassemblies and actions of the first supernode.
    // The actions correspond to all possible moves we can take in the first supernode.
    new_root->data.subassemblies[graph.root->data.name] = graph.root->id;
    for (auto &x : graph.getSuccessorNodes(graph.root->id))
    {
        new_root->data.actions[x->data.name] = x->id;
    }

    // Create the NodeExpander and pass it to the AStarSearch.
    // The AStarSearch uses the received Expander later during the search.
    // If a different expansion-behavior is desired, just modify the exapnder,
    // obeying to the interface used by the AStarSearch.
    NodeExpander expander(graph, search_graph, config);

    // Run search
    AStarSearch astar(graph);
    Node<SearchData> *result = astar.search(search_graph, new_root, expander);

    // Track the retrieved optimal assebly sequence
    std::vector<Task*> optimum;

    // Sequence of all agent-actions for the complete solution
    assembly_plan_.clear();

    // Backtrack the found optimum assembly-sequence
    double cost = 0;
    while (search_graph.hasPredecessor(result->id))
    {
        for (auto &i : search_graph.getPredecessorEdges(result->id).front()->data.planned_assignments)
        {
            double cur_cost = config.actions[i.action].costs[i.agent];
            std::cout << "Action: " << i.action << " Agent: " << i.agent << "    ";
            cost += cur_cost;
        }
        assembly_plan_.push_back(optimum);
        optimum.clear();
        std::cout << std::endl;
        result = search_graph.getPredecessorNodes(result->id).front();
    }

    std::cout << "Cost: " << cost << std::endl << std::endl;
    std::cout << "- - - -  - - - -  - - - -  - - - -  - - - -  - - - - " << std::endl << std::endl;

    return assembly_plan_;
}
