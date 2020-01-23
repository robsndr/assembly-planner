#pragma once

#include <iostream>
#include <unordered_map>
#include "dotwriter.hpp"
#include "astar.hpp"

class Planner
{
public:
    Planner();
    ~Planner();

    void operator()(Graph<> *, Node *, Config *);
    std::vector<std::vector<std::tuple<std::string, std::string, double>>> assembly_plan_;

private:
    std::vector<Node *> nodes_to_expand;
    std::vector<Node *> ors;

    Graph<> *search_graph;
};

Planner::Planner() {}

Planner::~Planner() {}

void Planner::operator()(Graph<> *graph, Node *root, Config *config)
{

    search_graph = new Graph<>();
    Node *new_root = search_graph->insertNode(root->data_);
    new_root->data_.subassemblies[root->data_.name] = root;

    for (auto &x : root->getSuccessorNodes())
    {
        new_root->data_.actions[x->data_.name] = x;
    }

    NodeExpander *expander = new NodeExpander(search_graph, config);

    AStarSearch astar;
    Node *result = astar.search(search_graph, new_root, expander);

    std::vector<std::tuple<std::string, std::string, double>> optimum;
    assembly_plan_.clear();

    double cost = 0;
    while (result->hasPredecessor())
    {
        for (auto &i : result->getPredecessors().front()->data_.agent_actions_)
        {
            std::cout << "Action: ";
            std::cout << i.first;
            std::cout << " Agent: ";
            std::cout << i.second << "    ";
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
}
