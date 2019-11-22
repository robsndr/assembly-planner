#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP

#include <string>
#include <cmath>
#include<set>
#include <vector>
#include "node.hpp"
#include <unordered_map>

enum class NodeType{AND, OR};

class Node;

class NodeData{
public:

    NodeData(){}

    NodeData(const NodeData & copied){
        name     = copied.name;
        worker   = copied.worker;
        cost     = copied.cost;
        type     = copied.type;
        marked   = copied.marked;
        subassemblies   = copied.subassemblies;
        actions         = copied.actions;
        minimum_cost_action = copied.minimum_cost_action;
        f_score  = 0;
        g_score  = 0;
    }

    bool isGoal();
    void calc_hscore();
    void calc_fscore();

    double cost = 0;
    NodeType type;

    std::string name = "";
    std::string worker = "";
;
    bool marked = false;
    bool goal = false;

    double g_score = 0;
    double f_score = 0;
    double h_score = 0;

    double minimum_cost_action = 0;

    std::unordered_map<std::string, Node*> subassemblies;
    std::unordered_map<std::string, Node*> actions;

};

// typedef std::size_t EdgeData;

class EdgeData {
public:
    EdgeData(){}

    std::vector< std::pair<std::string, std::string> > agent_actions_;
    double cost = 0;
};

class CostMap{
public:

    CostMap(){}

    CostMap(CostMap & parent){
        number_of_agents_ = parent.number_of_agents_;
        number_of_actions_ = parent.number_of_actions_;
    }

    CostMap(std::size_t actions, std::size_t agents){
        number_of_actions_ = actions;
        number_of_agents_  = agents;
    };

    void addMapping(std::string action, std::string agent, double cost){
        // Lets insert four elements
        const bool is_in = set_of_agents_.find(agent) != set_of_agents_.end();
        if(!is_in)
            vector_of_agents_.push_back(agent);    
        set_of_agents_.insert(agent);
        map_[action][agent] = cost;
    };

    std::size_t number_of_actions_;
    std::size_t number_of_agents_;

    std::vector<std::string> vector_of_agents_;
    std::set<std::string> set_of_agents_;

    std::unordered_map< std::string, std::unordered_map< std::string, double > > map_;

};

#endif