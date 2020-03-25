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
    std::unordered_map<std::string, ExecAgent*> agents_;

public:
    Supervisor(config::Configuration *);
    ~Supervisor();
    bool run(std::vector< std::vector< Task*>> & plan);
};

Supervisor::Supervisor(config::Configuration * config)
{
    endpoint_ = new WebsocketEndpoint;
    for(auto kv : config->agents) {
        config::Agent temp_agent = kv.second;
        agents_[temp_agent.name] = new ExecAgent(endpoint_, temp_agent.hostname, temp_agent.port);
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