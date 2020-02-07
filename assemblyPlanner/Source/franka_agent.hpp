#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "task.hpp"
#include "graph.hpp"

class FrankaAgent
{
public:
    FrankaAgent(Graph<> *);
    ~FrankaAgent();
    bool exec(std::vector< std::vector< Task*>> &);
private:
    std::vector<Node*> available_leaves;
    mongocxx::client *db_client;
};

FrankaAgent::FrankaAgent(Graph<> * ao_graph)
{

    available_leaves = ao_graph->getLeafNodes();

    for( auto node : available_leaves)
    {
        std::cout << node->data_.name << std::endl;
    }

    mongocxx::instance inst{};
    db_client = new mongocxx::client{mongocxx::uri{"mongodb://collective-panda-012:27017"}};
}

FrankaAgent::~FrankaAgent()
{
}

bool FrankaAgent::exec(std::vector< std::vector< Task*>> & plan){

    for(auto step : plan)
    {
        for(auto var: step)
        {
            var->getInfoFromDatabase(*db_client);
        }
        std::cout << std::endl;
    }

    return true;
}

// bool FrankaAgent::canTakeAction(Node* action){

// }