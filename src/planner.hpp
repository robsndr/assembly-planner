#pragma once

#include <iostream>
#include <unordered_map>
#include "dotwriter.hpp"
#include "astar.hpp"
#include "task.hpp"

/* Planner Class. 
    Used as a Top_Level supervisor for the planning process.
    Instantiates the AStarSearch, NodeExpander and search_graph.
**/
class Planner
{
public:
    // Start Planning
    std::vector< std::vector<Task*>> operator()(Graph<NodeData,EdgeData>, config::Configuration& );

private:
    // Container used to track the resulting optimal assembly sequence.
    // Vector of Tuples containing <action_pointer, agent_name, cost>
    std::vector<std::vector<Task*>> assembly_plan_;
    Graph<NodeData,EdgeData> *search_graph;
};

/* Start Plannning.
    @graph: pointer to the original A/O graph obtained from the InputReader
    @root: pointer to the node the search should start at.
    @config: configuration contianing the cost_map and reachability_map.
    \return: vector containing the assembly plan
**/
std::vector< std::vector<Task*>> 
Planner::operator()(Graph<NodeData,EdgeData> graph, config::Configuration& config)
{
    // Create a new Graph.
    // It is a different Graph the the one passed as a function parameter.
    // This one is the graph of Hypernodes used later for the A* search.
    Graph<NodeData,EdgeData> search_graph;
    auto new_root_id = search_graph.insertNode(graph.root->data_);
    Node<NodeData> *new_root = search_graph.getNode(new_root_id);

    // Set the subassemblies and actions of the first supernode.
    // The actions correspond to all possible moves we can take in the first supernode.
    new_root->data_.subassemblies[graph.root->data_.name] = graph.root->id;
    for (auto &x : graph.getSuccessorNodes(graph.root->id))
    {
        new_root->data_.actions[x->data_.name] = x->id;
    }

    // Create the NodeExpander and pass it to the AStarSearch.
    // The AStarSearch uses the received Expander later during the search.
    // If a different expansion-behavior is desired, just modify the exapnder,
    // obeying to the interface used by the AStarSearch.
    NodeExpander expander(graph, search_graph, config);

    // AStarSearch algorithm
    AStarSearch astar(graph);
    Node<NodeData> *result = astar.search(search_graph, new_root, expander);

    // Container used to represent the found agent-action assignement and its cost in a current step.
    // Vector of Tuples containing <action_pointer, agent_name, cost>
    std::vector<Task*> optimum;

    // Container representing the sequence of all agent-actions for the complete solution.
    assembly_plan_.clear();

    // Backtrack the found optimum assembly-sequence.
    double cost = 0;
    while (search_graph.numberOfPredecessors(result->id) > 0)
    {
        for (auto &i : search_graph.getPredecessorEdges(result->id).front()->data_.planned_assignments)
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
