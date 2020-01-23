#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "graph.hpp"

class FrankaAgent
{
public:
    FrankaAgent(Graph<> *);
    ~FrankaAgent();

    bool exec(std::vector<std::vector<std::tuple<std::string, std::string, double>>> &);

private:
    std::vector<Node*> available_actions;
};

FrankaAgent::FrankaAgent(Graph<> * ao_graph)
{
    std::vector<Node*> available_leaves;
    available_leaves = ao_graph->getLeafNodes();
    for(auto leaf : available_leaves)
    {
        available_actions.push_back(leaf->getPredecessorNodes()[0]);
        std::cout << "Node: " << leaf->data_.name << std::endl;
    }
}

FrankaAgent::~FrankaAgent()
{
}

bool FrankaAgent::exec(std::vector<std::vector<std::tuple<std::string, std::string, double>>> & plan){
    
    for(auto step : plan)
    {
        for(auto var : step)
        {
            std::cout << std::get<0>(var) << std::endl;
        }
    }

}
