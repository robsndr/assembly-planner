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

    bool isGoal();
    void calc_hscore();
    void calc_fscore();

    double cost = 0;
    NodeType type;

    std::string name = "";

    bool marked = false;

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

    std::vector<std::pair<Node*, std::string>> agent_actions_;
    double cost = 0;
};

namespace config{
    struct Parameter{
        std::string name;
        std::string value;
    };

    struct Task{
        std::string name;
        std::vector<Parameter> params;
    };

    struct Action{
        std::string name;
        Task task;
        std::unordered_map<std::string, double> costs;
    };

    struct Subassembly{
        std::string name;
        std::unordered_map<std::string, std::pair<bool, std::string>> reachability;
    };
    
    struct Agent{
        std::string name;
        std::string hostname;
        std::string port;
    };

    struct Configuration{
        std::unordered_map<std::string, Agent>  agents;
        std::unordered_map<std::string, Action> actions;
        std::unordered_map<std::string, Subassembly> subassemblies;
    };
}

