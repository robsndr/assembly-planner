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
    Graph<AssemblyData,EdgeData> operator()(Graph<AssemblyData,EdgeData>, config::Configuration& );
};

// Start planning
//   @graph:  pointer to the original A/O graph obtained from the InputReader
//   @config: configuration contianing the cost_map and reachability_map
//   \return: vector containing the assembly plan
//
Graph<AssemblyData,EdgeData>
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
    Graph<AssemblyData,EdgeData> assembly_plan;

    // Backtrack the found optimum assembly-sequence
    double cost = 0;

    std::unordered_map<NodeIndex, NodeIndex> idxs;

    while (search_graph.hasPredecessor(result->id))
    {
        for (auto &assignment : search_graph.getPredecessorEdges(result->id).front()->data.planned_assignments)
        {
            double cur_cost = config.actions[assignment.action].costs[assignment.agent];
            std::cout << "Action: " << std::setw(5) << assignment.action
                        << " Agent: " << std::setw(5) << assignment.agent << std::endl;

            if(graph.getNodeData(assignment.action_node_id).type == NodeType::INTERACTION)
            {
                auto x = graph.getNode(assignment.action_node_id);
                graph.eraseEdge(x->data.interaction_prev, x->data.interaction_next);
                graph.insertEdge(EdgeData(), x->data.interaction_prev, x->data.interaction_or);
            }

            cost += cur_cost;

            auto action_id = assembly_plan.insertNode(graph.getNodeData(assignment.action_node_id));
            
            for(auto &x: graph.getPredecessorNodes(assignment.action_node_id))
            {
                if(!idxs.count(x->id))
                {
                    auto prime_id = assembly_plan.insertNode(x->data);
                    idxs.insert(std::make_pair(x->id, prime_id));
                }
                assembly_plan.insertEdge(EdgeData(), idxs.at(x->id), action_id);
            }

            for(auto &x: graph.getSuccessorNodes(assignment.action_node_id))
            {
                if(!idxs.count(x->id))
                {
                    auto prime_id = assembly_plan.insertNode(x->data);
                    idxs.insert(std::make_pair(x->id, prime_id));
                }

                assembly_plan.insertEdge(EdgeData(), action_id, idxs.at(x->id));
            }
        }
        std::cout << std::endl;
        result = search_graph.getPredecessorNodes(result->id).front();
    }

    std::cout << "Cost: " << cost << std::endl << std::endl;
    std::cout << "- - - -  - - - -  - - - -  - - - -  - - - -  - - - - " << std::endl << std::endl;

    return assembly_plan;
}
