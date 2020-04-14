#pragma once

#include <vector>
#include <set>

#include "combinator.hpp"
#include "graph.hpp"
#include "types.hpp"

// During the A* search the supernodes need to be expanded.
// The `NodeExpander` expands and creates new hypernodes for the A* search graph.
// The search itself is performed on a graph of hyper-nodes.
// Every hypernode contains references to the nodes which are part of the assembly graph.
// Interactions are added to the hypernodes by the `NodeExpander`, if necessary.
struct NodeExpander
{
    NodeExpander(Graph<AssemblyData,EdgeData>&, 
        Graph<SearchData,EdgeData>&, config::Configuration&);

    void expandNode(NodeIndex);

  private:
    // Create interaction nodes if subassemblies are not reachable.
    NodeIndex createInteraction(NodeIndex, NodeIndex, AssemblyData&, std::string);
    // Assembly
    Graph<AssemblyData,EdgeData>& assembly_graph_;
    // Hypergraph used for search
    Graph<SearchData,EdgeData>& search_graph_;
    // Pointers to cost/reach maps provided by the InputReader.
    config::Configuration& config;
    // Assignment generation object
    Combinator assignment_generator_;
};

NodeExpander::NodeExpander(Graph<AssemblyData,EdgeData>& assembly_graph, 
        Graph<SearchData,EdgeData>& search_graph, config::Configuration& conf)
  : config(conf),
    assignment_generator_(config),
    assembly_graph_(assembly_graph),
    search_graph_(search_graph)
{}

// Node expansion: the A* search algorithm calls `expandNode` when a new hypernode is processed.
// The nodes produeced by this function, are introduced into the search graph as result.
void NodeExpander::expandNode(NodeIndex node_id)
{
    std::vector<NodeIndex> nodes;

    auto& node_data = search_graph_.getNodeData(node_id);
    for (const auto& sa : node_data.subassemblies)
    {
        if (assembly_graph_.hasSuccessor(sa.second))
            nodes.push_back(NodeIndex(sa.second));
    }

    // Obtain all possible combinations of agents-action assignments for the current step
    const auto assignments_ = assignment_generator_.generateAgentActionAssignments(assembly_graph_, nodes);
    
    // Min/Max used to track the optimum across agent-action assignemnts
    double min_action_agent_cost_ = node_data.minimum_cost_action;
    double max_action_agent_cost_ = 0;

    // Iterate through all possible assignments of agents to available actions
    for (const auto& cur_assignments : assignments_)
    {
        // Create the data for the created supernode.
        // Temporary node data
        SearchData x;
        x.subassemblies = node_data.subassemblies;
        x.actions = node_data.actions;
        x.marked = false;
        // Temporary edge data
        EdgeData y;
        y.cost = 0;

        // Needed to calculate the average cost for the connecting edge.
        int iters = 0;

        // Iterate through agent-action pairs for the current assignemnt
        for (const auto& assignment : cur_assignments)
        {
            iters++;

            const auto& agent = assignment.agent;
            const auto& action = assignment.action;
            const auto& action_node_id = assignment.action_node_id;

            // Update the data for the newly-created supernode.
            auto action_source_id = assembly_graph_.predecessorNodes(action_node_id).front();
            auto action_source = assembly_graph_.getNodeData(action_source_id).name;
            x.subassemblies.erase(action_source);
            x.actions.erase(action);

            // For the currently applied assignement, update the subassemblies of the new supernode.
            for (auto& successor_id : assembly_graph_.successorNodes(action_node_id))
            {
                auto successor = assembly_graph_.getNodeData(successor_id);
                NodeIndex ors_prime = successor_id;

                bool part_reachable = config.subassemblies[successor.name].reachability[agent].reachable;

                if (!part_reachable)
                {
                    // Part not reachable - add interaction
                    auto interaction = config.subassemblies[successor.name].reachability[agent].interaction.name;
                    ors_prime = createInteraction(action_node_id, successor_id, successor, interaction);
                }

                x.subassemblies[successor.name] = ors_prime;

                for (const auto& next_action_id : assembly_graph_.successorNodes(ors_prime))
                {
                    auto next_action = assembly_graph_.getNodeData(next_action_id);
                    x.actions[next_action.name] = next_action_id;
                }
            }

            // Update the minimum cost which can be achieved by any agent for any available action.
            if (config.actions[action].costs[agent] < min_action_agent_cost_)
            {
                min_action_agent_cost_ = config.actions[action].costs[agent];
            }

            // Update edge data.
            y.cost += config.actions[action].costs[agent];
            y.planned_assignments.push_back(assignment);
        }

        // Create the average of the edge.cost over the number of nodes it connects.
        // Every node in the search graph is a hyper-node produced by many nodes from the assembly.
        // This makes taking the average over the number of represented nodes necessary.
        y.cost = y.cost / iters;

        // Insert the newly created sueprnode into the search graph.
        auto next_node_id = search_graph_.insertNode(x);
        search_graph_.insertEdge(y, node_id, next_node_id);
    }

    // Set the minum-agen-action cost for the curent supernode.
    // Needed for the heuristic used by the A* algorithm.
    node_data.minimum_cost_action = min_action_agent_cost_;
}

// Interactions are created for assignemnts where a given agent cannot reach a part (subassembly).
NodeIndex NodeExpander::createInteraction(NodeIndex src_id, NodeIndex dest_id, 
                    AssemblyData& dest_data, std::string iname)
{
    // Create interaction subassembly cotaining the same data as original one
    AssemblyData tdata = dest_data;
    tdata.name = dest_data.name + "′";
    tdata.type = NodeType::INTERASSEMBLY;
    auto or_prime_id = assembly_graph_.insertNode(tdata);
    // Create node for interaction
    AssemblyData idata;
    idata.name = iname;
    idata.type = NodeType::INTERACTION;
    // Set pointer of action that triggered interaction 
    // Needed for backtracking the optimal solution at the planner level, 
    // since interaction are not inserted directly into the graph, 
    // but are an overlay layer on top of it, with a forward-path only
    idata.interaction_prev = src_id;
    idata.interaction_or = or_prime_id;
    idata.interaction_next = dest_id;

    auto interaction_id = assembly_graph_.insertNode(idata);
    // Insert interaction between corresponding nodes
    assembly_graph_.insertEdge(EdgeData(), or_prime_id, interaction_id);
    assembly_graph_.insertEdge(EdgeData(), interaction_id, dest_id);
    // Return the interaction subassembly to insert into the current supernode.
    return or_prime_id;
}
