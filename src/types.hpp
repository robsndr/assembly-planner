#pragma once

#include <iostream>

using NodeIndex = size_t;
using EdgeIndex = size_t;
struct AgentActionAssignment {
    std::string agent; 
    std::string action; 
    size_t action_node_id;
};