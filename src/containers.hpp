#pragma once

#include <string>
#include <ostream>
#include <cmath>
#include <set>
#include <vector>
#include "node.hpp"
// #include "graph.hpp"
#include <unordered_map>
#include "types.hpp"

enum class NodeType
{
    AND,
    OR
};

class NodeData
{
public:
    NodeData() {}

    double cost = 0;
    NodeType type;

    std::string name = "";

    bool marked = false;

    double g_score = 0;
    double f_score = 0;
    double h_score = 0;

    double minimum_cost_action = MAXFLOAT;

    std::unordered_map<std::string, size_t> subassemblies;
    std::unordered_map<std::string, size_t> actions;
};

// typedef std::size_t EdgeData;
class EdgeData
{
public:
    EdgeData() {}

    std::vector<AgentActionAssignment> planned_assignments;
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

    struct Reach{
        bool reachable;
        std::string interaction;
    };

    struct Subassembly{
        std::string name;
        std::unordered_map<std::string, Reach> reachability;
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

