#include <string>
#include <cmath>

enum class NodeType{AND, OR};

class NodeData{
public:
    double cost = 0;
    NodeType type;

    std::string name = "";
    std::string worker = "";

    bool expanded = false;
    bool marked = false;
    bool solved = false;;
    bool solution = false;

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
        cost_map_[action][agent] = cost;
    };

    std::size_t number_of_actions_;
    std::size_t number_of_agents_;
    std::unordered_map< std::string, std::unordered_map< std::string, double > > cost_map_;

};