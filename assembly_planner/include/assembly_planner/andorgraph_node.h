#pragma once

#include <list>
#include <algorithm>
#include <string>

#include "assembly_planner/math_functions.h"
#include "assembly_planner/a_star.h"
#include "assembly_planner/data_Assembly.h"
//#include "assembly_planner/planner.h"

struct Node_Data{

    std::set<And_node*> and_nodes;
    std::map<std::string,std::string> assignmentMap_worker2action;
    std::set<Or_node*> leafs;
    Memory* memory;
    std::map<std::string,std::list<AssemblyAction> > sequence;
    std::map<std::string,std::set<std::string> > skills;

};

class AndOrGraph_node : public algorithms::search::AStar_node
{
public:
    // simple constructor for start and goal nodes
    AndOrGraph_node(std::set<Or_node*> or_nodes,Memory* memory,std::map<std::string,std::set<std::string> > skills);
    AndOrGraph_node(Node_Data data);
    ~AndOrGraph_node();

    double heuristic(AStar_node* node);
    bool equalsNode(AStar_node* node);
    std::list<AStar_node*> getNeighbors();
    std::string getUniqueString();
    bool goalTest(AStar_node* node);
    double getActionCost(AStar_node* node);

    std::set<And_node*>* getAndNodes() ;
    std::set<Or_node*>* getOrNodes();
    std::set<And_node*>* getActions();
    std::map<std::string,double> getDuration();
    std::map<std::string,std::string> getMappingWorkerToActions();
    std::map<std::string,std::list<AssemblyAction> > getActionSequence();

private:

    std::map<std::string,Object_Worker*> getWorker();

	double getCost(std::string type_worker, std::string type_action);

    unsigned assignAgents(std::vector<std::vector<unsigned> > &assignment_global, std::vector<bool> &assigned, unsigned agent, bool newAssignment, unsigned assignment_cnt, unsigned nAndnodes, unsigned nWorker);
    std::set<Or_node*>::iterator pickActions(std::vector<std::vector<std::string> >& combs,std::vector<std::string>& current, std::set<Or_node*>::iterator it_or);

    Memory* _memory;
    std::map<std::string,Object_Worker*> _worker;
    std::map<std::string,std::set<std::string> > _skills;

    ANDORgraph* _graph;

	std::map<std::string, std::list<AssemblyAction> > _sequence;

    // contains all possible actions
    std::set<And_node*> _and_nodes;
    std::set<Or_node*> _or_nodes;
    std::set<std::string> _actions;
    std::map<std::string,std::string> _assignmentMap_worker_to_actions;

};
