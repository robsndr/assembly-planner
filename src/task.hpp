#pragma once

#include <iostream>
#include <string>

struct Task
{
    Task(std::string action, std::string agent, double cost)
    {
        agent_ = agent;
        action_ = action;
        cost_ = cost;
    };

    bool exec()
    {
        return true;
    };

    friend std::ostream & operator<<(std::ostream &os, const Task& p)
    {
        os << "* Agent: " << p.agent_ << "  | Action: " << p.action_ << std::endl;
        return os;
    };

    std::string action_;
    std::string agent_; 
    double cost_;
};
