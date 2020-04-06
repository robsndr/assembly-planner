#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include "graph.hpp"
#include "types.hpp"

struct Combinator
{
    Combinator(config::Configuration &);

    std::vector<std::vector<AgentActionAssignment>>
    generateAgentActionAssignments(Graph<AssemblyData, EdgeData> &, std::vector<NodeIndex> &);

private:
    void printAssignments();
    void assignAgentsToActions(std::vector<std::string> &, std::vector<std::tuple<std::string, NodeIndex>> &);
    void generateActionCombinationSets(Graph<AssemblyData, EdgeData> &, std::vector<NodeIndex> &);
    void generateAgentCombinationSets(std::vector<std::string> &, int);

    // All combinations of agent-action assigments for a given assembly node
    std::vector<std::vector<AgentActionAssignment>> agent_action_assignements_;

    // First entry denotes node, second entry denotes actions
    std::unordered_map<std::string, std::vector<std::string>> node_actions_;

    // Data Structures needed to calculate the agent combinations
    // Defines as class-wide objects to facilitate reuse between iterations.
    std::vector<std::vector<std::string>> temp_agent_combinations_;
    std::vector<std::string> temp_agent_set_;

    // Data Structures needed for the Action-Combination Generation.
    // Defined here to create them once and reuse without repeating allocation.
    std::vector<std::vector<std::tuple<std::string, NodeIndex>>> temp_action_combinations_;
    std::vector<std::tuple<std::string, NodeIndex>> temp_action_set_;

    config::Configuration &config_;
};

Combinator::Combinator(config::Configuration &config)
    : config_(config)
{}

// Function which performs the generation of assignments of workers to actions.
std::vector<std::vector<AgentActionAssignment>>
Combinator::generateAgentActionAssignments(Graph<AssemblyData, EdgeData> &graph, std::vector<NodeIndex> &nodes)
{
    std::size_t l = std::min(nodes.size(), config_.agents.size());

    generateActionCombinationSets(graph, nodes);

    agent_action_assignements_.clear();

    std::vector<std::string> vector_of_agents;
    for (auto &key_value : config_.agents)
    {
        vector_of_agents.push_back(key_value.second.name);
    }

    for (size_t j = 1; j <= l; j++)
    {
        generateAgentCombinationSets(vector_of_agents, j);

        for (auto &agents : temp_agent_combinations_)
        {
            for (auto &actions : temp_action_combinations_)
            {
                assignAgentsToActions(agents, actions);
            }
        }
    }
    // printAssignments();
    return agent_action_assignements_;
}

// Function which performs the generation of assignments of workers to actions.
void Combinator::assignAgentsToActions(std::vector<std::string> &cur_agents,
                                       std::vector<std::tuple<std::string, NodeIndex>> &cur_actions)
{
    int n = cur_actions.size();
    int k = cur_agents.size();

    std::vector<AgentActionAssignment> assignment;

    // Create selector vector
    std::vector<int> d(n);
    std::iota(d.begin(), d.end(), 0);
    do
    {
        for (int i = 0; i < k; i++)
        {
            assignment.push_back(AgentActionAssignment{
                .agent = cur_agents[i],
                .action = std::get<0>(cur_actions[d[i]]),
                .action_node_id = std::get<1>(cur_actions[d[i]])
            });
        }
        agent_action_assignements_.push_back(assignment);
        assignment.clear();
        std::reverse(d.begin() + k, d.end());
    } 
    while (next_permutation(d.begin(), d.end()));
}

// Function which performs the generation of assignments of workers to actions.
void Combinator::generateAgentCombinationSets(std::vector<std::string> &agents, int k)
{
    int n = agents.size();
    temp_agent_combinations_.clear();

    // Create selector vector
    std::vector<bool> v(n);
    std::fill(v.begin(), v.begin() + k, true);
    // Generate agent-action permutations
    do
    {
        temp_agent_set_.clear();
        for (int i = 0; i < n; ++i)
        {
            if (v[i])
            {
                temp_agent_set_.push_back(agents[i]);
            }
        }
        temp_agent_combinations_.push_back(temp_agent_set_);
    } while (std::prev_permutation(v.begin(), v.end()));
}

// Function which performs the generation the possible action combinations
void Combinator::generateActionCombinationSets(Graph<AssemblyData, EdgeData> &graph,
                                               std::vector<NodeIndex> &node_ids)
{
    // Clear action combinations
    temp_action_combinations_.clear();

    // number of arrays
    int n = node_ids.size();
    int indices[n];

    // initialize with first element's index
    for (int i = 0; i < n; i++)
        indices[i] = 0;

    while (1)
    {
        temp_action_set_.clear();

        for (int i = 0; i < n; i++)
        {
            auto nodes = graph.successorNodes(node_ids[i])[indices[i]];
            const auto &action = graph.getNodeData(nodes);
            temp_action_set_.push_back(std::make_tuple(action.name, nodes));
        }
        temp_action_combinations_.push_back(temp_action_set_);
        // Find the rightmost array that has more elements left
        // after the current element in that array
        int next = n - 1;
        while (next >= 0 &&
               (indices[next] + 1 >= graph.numberOfSuccessors(node_ids[next])))
            next--;

        // no such array is found so no more
        // combinations left
        if (next < 0)
            break;

        // if found move to next element in that
        // array
        indices[next]++;

        // for all arrays to the right of this
        // array current index again points to
        // first element
        for (int i = next + 1; i < n; i++)
            indices[i] = 0;
    }
}

// Debug: prints the current state of the assignment vector
void Combinator::printAssignments()
{
    std::cout << std::endl
              << "******************Current assignments: ***********************"
              << std::endl;

    for (auto const &assignment : agent_action_assignements_)
    {
        for (auto const &elem : assignment)
        {
            std::cout << elem.action << " : " << elem.agent << std::endl;
        }
        std::cout << "--------------" << std::endl;
    }
    std::cout << "**************************************************************"
              << std::endl;
}
