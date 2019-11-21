#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP


#include <string>
#include <cmath>
#include<set>
#include <vector>
#include "node.hpp"

class Node;

enum class NodeType{AND, OR};

class NodeData{
public:

    NodeData(){}

    NodeData(const NodeData & copied){
        name     = copied.name;
        worker   = copied.worker;
        cost     = copied.cost;
        type     = copied.type;
        marked   = copied.marked;
        f_score  = INT_MAX;
        g_score  = INT_MAX;
    }

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

    std::vector<Node*> subassemblies;
    std::vector<Node*> actions;


    bool isGoal(){
        return goal;     
    }

    void calc_hscore(){
        h_score = log2f(name.length());
    }

    void calc_fscore(){
        f_score = g_score + h_score;
    }

    void reset(){
        // marked = false;
        if(type == NodeType::OR)
            cost = log2(name.length());
    }
};

// typedef std::size_t EdgeData;

class EdgeData {
public:
    EdgeData(){}

    std::string worker = "";
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