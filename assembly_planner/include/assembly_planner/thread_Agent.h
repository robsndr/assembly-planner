#pragma once
#include <vector>
#include <string>
#include <iostream>

#include <ros/ros.h>

#include <boost/thread.hpp>
#include <boost/signals2/signal.hpp>

#include <assembly_planner/data_Assembly.h>

enum Status{idle,busy,finished,error};

class Thread_Agent
{
public:
    Thread_Agent();
    Thread_Agent(Object_Worker* w);
    ~Thread_Agent();

    void startTask(AssemblyAction a);
    void finishedTask(std::string a);

    void confirmFinishedTask();


    Status getStatus();
    void setStatus(Status s);

    void lockMutex();
    void unlockMutex();

    void signal_startTask(AssemblyAction a);

    boost::signals2::signal<void (AssemblyAction)> _sig_start;


private:

    Status _status;

    boost::mutex _mtx;

    Object_Worker* _agent;

};

class AgentWrapper{
public:

    AgentWrapper();
    AgentWrapper(std::string id);
    ~AgentWrapper();
    void startTask(AssemblyAction a);
    void signal_finished(std::string res);

    virtual void setup() = 0;
    virtual bool resetAgent() = 0;

    boost::signals2::signal<void (std::string)> _sig_finished;

protected:
    std::string getAssemblyPart(std::set<std::string> parts);
    std::set<std::string> splitString(std::string str, std::string delimiter);

    virtual void executeAction(AssemblyAction) = 0;

    std::string _id;
};
