#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP


#include <string>
#include <cmath>
#include<set>

enum class NodeType{AND, OR};

class NodeData{
public:

    NodeData(){}

    NodeData(const NodeData & copied){
        name     = copied.name;
        worker   = copied.worker;
        expanded = copied.expanded;
        cost     = copied.cost;
        type     = copied.type;
        marked   = copied.marked;
        solved   = copied.solved;
        solution = copied.solution;

    }

    double cost = 0;
    NodeType type;

    std::string name = "";
    std::string worker = "";

    bool expanded = false;
    bool marked = false;
    bool solved = false;;
    bool solution = false;

    bool debug = false;

    void reset(){
        // marked = false;
        if(type == NodeType::OR)
            cost = log2(name.length());

        solved = false;
        solution = false;
    }
};

typedef std::size_t EdgeData;


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