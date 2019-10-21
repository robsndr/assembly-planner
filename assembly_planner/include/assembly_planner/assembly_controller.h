#ifndef ASSEMBLY_CONTROLLER_H
#define ASSEMBLY_CONTROLLER_H

#include <math.h>

#include <ros/ros.h>

#include "assembly_planner/thread_Agent.h"
#include "assembly_planner/planner.h"


class Assembly_Controller{
public:

    Assembly_Controller();
    ~Assembly_Controller();

    void assemblyProcess();
    void setupController(Memory* mem, ANDORgraph* graph, std::set<Object_Worker> worker);
    void setSkills(std::map<std::string,std::set<std::string> > skills);
    void resetGraph(ANDORgraph* graph);

    void setupStandardInteractionCosts(double val);

    std::map<std::string,Thread_Agent*>* getAgents();

private:

    void setupAssemblies(std::set<Object_Assembly> assemblies);
    void setupWorker(std::set<Object_Worker> worker);
    void setupAndOrGraph(ANDORgraph* graph);

    bool isApplicable(AssemblyAction action);
    std::set<std::string> joinSubAssemblies(std::set<std::string> currentParts, std::set<Or_node*> subassemblies);
    void startInteraction(AssemblyAction a);
    void finishInteraction(AssemblyAction a);

	std::set<std::string> splitString(std::string str, std::string delimiter);

    std::map<std::string,Thread_Agent*> _agent_threads;
    boost::mutex _mtx;

    std::map<std::string,std::set<std::string> > _skills;

    Memory* _memory;
    ANDORgraph* _graph;
    Planner* _planner;


};


#endif // ASSEMBLY_CONTROLLER_H
