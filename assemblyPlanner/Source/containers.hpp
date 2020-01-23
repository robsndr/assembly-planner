#pragma once

#include <string>
#include <ostream>
#include <cmath>
#include <set>
#include <vector>
#include "node.hpp"
#include <unordered_map>

enum class NodeType
{
    AND,
    OR
};

class Node;

class NodeData
{
public:
    NodeData() {}

    bool isAtomic()
    {
        return atomic;
    };
    bool isGoal();
    void calc_hscore();
    void calc_fscore();

    double cost = 0;
    NodeType type;

    std::string name = "";
    std::string worker = "";

    bool marked = false;
    bool atomic = false;

    double g_score = 0;
    double f_score = 0;
    double h_score = 0;

    double minimum_cost_action = MAXFLOAT;

    std::unordered_map<std::string, Node *> subassemblies;
    std::unordered_map<std::string, Node *> actions;
};

// typedef std::size_t EdgeData;
class EdgeData
{
public:
    EdgeData() {}

    std::vector<std::pair<std::string, std::string>> agent_actions_;
    double cost = 0;
};

template <class temptype = std::size_t>
class BaseMap
{
public:
    BaseMap() {}

    virtual void addMapping(std::string, std::string, temptype) = 0;

    std::unordered_map<std::string, std::unordered_map<std::string, temptype>> map_;

    std::vector<std::string> vector_of_agents_;
    std::size_t number_of_agents_;
    std::set<std::string> set_of_agents_;
};

class CostMap : public BaseMap<double>
{

public:
    void addMapping(std::string action, std::string agent, double cost)
    {
        // Lets insert four elements
        bool is_in = set_of_agents_.find(agent) != set_of_agents_.end();
        if (!is_in)
            vector_of_agents_.push_back(agent);
        set_of_agents_.insert(agent);

        is_in = set_of_actions_.find(action) != set_of_actions_.end();
        if (!is_in)
            vector_of_actions_.push_back(action);
        set_of_actions_.insert(action);

        map_[action][agent] = cost;
        number_of_actions_ = vector_of_actions_.size();
        number_of_agents_ = vector_of_agents_.size();
    };

    std::size_t number_of_actions_;
    std::vector<std::string> vector_of_actions_;
    std::set<std::string> set_of_actions_;
};

class ReachMap : public BaseMap<std::pair<bool, std::string>>
{

public:
    void addMapping(std::string part, std::string agent, std::pair<bool, std::string> reach)
    {
        // Lets insert four elements
        bool is_in = set_of_agents_.find(agent) != set_of_agents_.end();
        if (!is_in)
            vector_of_agents_.push_back(agent);
        set_of_agents_.insert(agent);

        is_in = set_of_parts_.find(part) != set_of_parts_.end();
        if (!is_in)
            vector_of_parts_.push_back(part);
        set_of_parts_.insert(part);

        map_[part][agent] = reach;
        number_of_parts_ = vector_of_parts_.size();
        number_of_agents_ = vector_of_agents_.size();
    };

    std::size_t number_of_parts_;
    std::vector<std::string> vector_of_parts_;
    std::set<std::string> set_of_parts_;
};

class Config
{
public:
    CostMap *costs_;
    ReachMap *reach_;
};