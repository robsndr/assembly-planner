#include "assembly_planner/planner.h"



Planner::Planner(){}


Planner::~Planner(){}

void Planner::linkToMemory(Memory * memory){
    this->_memory = memory;
}

algorithms::search::AStar* Planner::getAstar(){
    return &this->_astar;
}


std::map<std::string, std::list<AssemblyAction> > Planner::calculateNominalSequence(std::set<std::string> currentSubassemblies){

    ANDORgraph* graph=static_cast<Object_ANDORgraph*>(this->_memory->getMemory("andorgraph"))->_graph;
    // get the pointers to the OR nodes of the current assembly status
    //    std::cout<<"listing current assemblies: "<<std::endl;
    std::set<Or_node*> currentOrNodes;
    for(std::string assembly : currentSubassemblies){
        currentOrNodes.insert(&graph->_or_nodes[assembly]);
    }

    // setup the start node
    AndOrGraph_node start({{graph->_root}},this->_memory,this->_skills);
    start.setPredecessor(NULL);
    // setup the goal node with the current assembly status
    AndOrGraph_node goal(currentOrNodes,this->_memory,this->_skills);

    // conduct the search
    std::cout<<"astar: seaching...";
    AndOrGraph_node* node = static_cast<AndOrGraph_node*>(this->_astar.search(&start,&goal));
    std::cout<<"done."<<std::endl;

    std::list<std::map<std::string,std::string> > actions_sequence;
    actions_sequence.resize(0);

    std::map<std::string, std::list<AssemblyAction> > action_sequence;


    // check if a solution has been obtained
    if(node==NULL){
        std::cout<<"No solution has been obtained."<<std::endl;
        return action_sequence;
    }
    action_sequence = node->getActionSequence();

    return action_sequence;
    return std::map<std::string, std::list<AssemblyAction>>();
}
