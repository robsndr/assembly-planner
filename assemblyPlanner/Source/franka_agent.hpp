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
    bool exec( std::vector< std::vector< std::tuple< Node*, std::string, double >>>& );
private:
    std::vector<Node*> available_leaves;
};

FrankaAgent::FrankaAgent(Graph<> * ao_graph)
{
    available_leaves = ao_graph->getLeafNodes();

    for( auto node : available_leaves)
    {
        std::cout << node->data_.name << std::endl;
    }
}

FrankaAgent::~FrankaAgent()
{
}

bool FrankaAgent::exec(std::vector< std::vector< std::tuple< Node*, std::string, double >>> & plan){
    
    for(auto step : plan)
    {
        for(auto var: step)
        {
            std::cout << std::get<0>(var)->data_.name << " ";
        }
        std::cout << std::endl;
    }

}

// bool FrankaAgent::canTakeAction(Node* action){

// }