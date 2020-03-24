#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>

#include "exec_agent.hpp"

class Supervisor
{
private:
    WebsocketEndpoint *endpoint_;
    std::unordered_map<std::string, std::string> configuration_;
    std::unordered_map<std::string, ExecAgent*> agents_;


public:
    Supervisor(std::unordered_map<std::string, std::string> &);
    ~Supervisor();
    bool run(std::vector< std::vector< Task*>> & plan);
};

Supervisor::Supervisor(std::unordered_map<std::string, std::string> & config)
{
    endpoint_ = new WebsocketEndpoint;
    configuration_ = config;
    for(auto kv : configuration_) {
        std::string agent_id = kv.first;
        std::string agent_host = kv.second;
        agents_[agent_id] = new ExecAgent(endpoint_, agent_host);
    } 
}

Supervisor::~Supervisor()
{
    for(auto kv : agents_) {
        ExecAgent* _ = kv.second;
        delete _;
    } 
    delete endpoint_;
}

bool Supervisor::run(std::vector< std::vector< Task*>> & plan){
    
    for(auto step : plan)
    {
        for(auto var: step)
        {
            agents_[var->agent_]->exec(var->action_);            
        }
        std::string input;
        std::cout << "Run step in Plan. Press button to continue.";
        std::getline(std::cin, input);
        std::cout << std::endl;
    }
    return true;
}