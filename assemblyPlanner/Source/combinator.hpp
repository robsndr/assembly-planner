#pragma once

#include <vector>
#include <string>
#include "graph.hpp"
#include <algorithm>
#include <numeric>

class Combinator
{

public:
    Combinator(CostMap *);
    ~Combinator();

    std::vector<std::vector<std::tuple<std::string, std::string, Node *>>> *
    generateAgentActionAssignments(std::vector<Node *> &);

private:
    void printAssignments();
    void assignAgentsToActions(std::vector<std::string> &, std::vector<std::tuple<std::string, Node *>> &);
    void generateActionCombinationSets(std::vector<Node *> &);
    void generateAgentCombinationSets(std::vector<std::string> &, int);

    std::unordered_map<std::string, std::vector<std::string>> node_actions_; // First entry denotes node.
                                                                             // Second entry denotes action.

    std::vector<std::string> temp_nodes;

    std::vector<std::vector<std::tuple<std::string, std::string, Node *>>> agent_action_assignements_;

    // Data Structures needed to calculate the agent combinations
    // Defines as class-wide objects to facilitate reuse between iterations.
    std::vector<std::vector<std::string>> temp_agent_combinations_;
    std::vector<std::string> temp_agent_set_;

    // Data Structures needed for the Action-Combination Generation.
    // Defined here to create them once and reuse without repeating allocation.
    std::vector<std::vector<std::tuple<std::string, Node *>>> temp_action_combinations_;
    std::vector<std::tuple<std::string, Node *>> temp_action_set_;

    CostMap *cost_map_;
};

Combinator::Combinator(CostMap *costs)
{
    cost_map_ = costs;
}

Combinator::~Combinator() {}

/* Function which performs the generation of assignments of workers to actions.
**/
std::vector<std::vector<std::tuple<std::string, std::string, Node *>>> *
Combinator::generateAgentActionAssignments(std::vector<Node *> &nodes)
{

    std::size_t l = std::min(nodes.size(), cost_map_->number_of_agents_);

    generateActionCombinationSets(nodes);

    agent_action_assignements_.clear();

    for (size_t j = 1; j <= l; j++)
    {
        // std::cout << "Brun" << std::endl;

        generateAgentCombinationSets(cost_map_->vector_of_agents_, j);

        for (auto &agents : temp_agent_combinations_)
        {
            for (auto &actions : temp_action_combinations_)
            {
                assignAgentsToActions(agents, actions);
            }
        }
    }

    // printAssignments();

    return &agent_action_assignements_;
}

/* Function which performs the generation of assignments of workers to actions.
**/
void Combinator::assignAgentsToActions(std::vector<std::string> &cur_agents,
                                       std::vector<std::tuple<std::string, Node *>> &cur_actions)
{

    int n = cur_actions.size();
    int k = cur_agents.size();

    std::vector<std::tuple<std::string, std::string, Node *>> assignment;

    // Create selector vector
    std::vector<int> d(n);
    std::iota(d.begin(), d.end(), 0);
    // std::cout << "These are the Possible Permutations: " << std::endl;
    do
    {
        for (int i = 0; i < k; i++)
        {
            // std::cout << cur_actions[d[i]] << " ";
            assignment.push_back(std::make_tuple(cur_agents[i],
                                                 std::get<0>(cur_actions[d[i]]),
                                                 std::get<1>(cur_actions[d[i]])));
        }
        agent_action_assignements_.push_back(assignment);
        // std::cout << std::endl;
        assignment.clear();
        std::reverse(d.begin() + k, d.end());
    } while (next_permutation(d.begin(), d.end()));
}

/* Function which performs the generation of assignments of workers to actions.
**/
void Combinator::generateAgentCombinationSets(std::vector<std::string> &agents, int k)
{

    int n = agents.size();
    temp_agent_combinations_.clear();

    // Create selector vector
    std::vector<bool> v(n);
    std::fill(v.begin(), v.begin() + k, true);
    do
    {
        temp_agent_set_.clear();
        for (int i = 0; i < n; ++i)
        {
            if (v[i])
            {
                // std::cout << agents[i] << " ";
                temp_agent_set_.push_back(agents[i]);
            }
        }
        temp_agent_combinations_.push_back(temp_agent_set_);
        // std::cout << "\n";
    } while (std::prev_permutation(v.begin(), v.end()));
}

/* Function which performs the generation the possible action combinations
**/
void Combinator::generateActionCombinationSets(std::vector<Node *> &nodes)
{

    temp_action_combinations_.clear();

    // number of arrays
    int n = nodes.size();
    int *indices = new int[n];

    // initialize with first element's index
    for (int i = 0; i < n; i++)
        indices[i] = 0;

    while (1)
    {
        temp_action_set_.clear();
        // print current combination
        for (int i = 0; i < n; i++)
        {
            Node *and_node = nodes[i]->getSuccessorNodes()[indices[i]];
            temp_action_set_.push_back(std::make_tuple(and_node->data_.name, and_node));
            // std::cout << temp_action_name << " ";
        }
        temp_action_combinations_.push_back(temp_action_set_);
        // std::cout << std::endl;

        // find the rightmost array that has more
        // elements left after the current element
        // in that array
        int next = n - 1;
        while (next >= 0 &&
               (indices[next] + 1 >= nodes[next]->numberOfSuccessors()))
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

    delete[] indices;
}

/* Debug functionality. Prints the current state of the assignment vector.
**/
void Combinator::printAssignments()
{

    std::cout << std::endl
              << "***************Print current assignments: ********************" << std::endl;

    for (auto const &assignment : agent_action_assignements_)
    {
        for (auto const &elem : assignment)
        {
            std::cout << std::get<0>(elem) << " : " << std::get<1>(elem)
                      // << " : " << std::get<2>(elem)->data_.name
                      << std::endl;
        }
        std::cout << "----------" << std::endl;
    }

    std::cout << "**************************************************************" << std::endl
              << std::endl;
}
