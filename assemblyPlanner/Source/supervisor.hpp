#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <pthread.h> 

#include "exec_agent.hpp"

class Supervisor
{
private:
    WebsocketEndpoint *endpoint_;
    std::vector< std::vector< Task*>> plan_;
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

    for(auto step : plan_){
        for(auto task: step){
            delete task;
        }
    }

    for(auto kv : agents_) {
        ExecAgent* _ = kv.second;
        delete _;
    } 
    delete endpoint_;
}

bool Supervisor::run(std::vector< std::vector< Task*>> & plan){
    
    plan_ = plan;
    std::cout << "Run in Supervisor." << std::endl;
    for(auto step : plan)
    {
        websocket_sync::semaphore.reset(step.size()); // number of threads to sync
        
        
        for(auto var: step)
        {
            agents_[var->agent_]->exec(var->action_);            
        }
        websocket_sync::semaphore.wait();

        std::cout << "Step Completed." << std::endl;
    }
    return true;
}