#include "assembly_planner/assembly_controller.h"


Assembly_Controller::Assembly_Controller(){

}

Assembly_Controller::~Assembly_Controller(){

}


void Assembly_Controller::assemblyProcess(){

    std::map<std::string,AssemblyAction> appliedActions;
    appliedActions.insert(std::pair<std::string,AssemblyAction>("none",AssemblyAction()));
    std::map<std::string, std::list<AssemblyAction> > sequence;
    std::map<std::string, std::list<AssemblyAction> > agent_sequences;
    std::map<std::string, AssemblyAction> currentActions;
    std::string status_planner = "replan";
    std::map<std::string,Object_Worker> worker=static_cast<Object_Map<Object_Worker>* >(this->_memory->getMemory("map_worker"))->_map;

    std::set<std::string> currentParts;
    for(Or_node* o : this->_graph->_leafs){
        currentParts.insert(o->getID());
    }

    this->_memory->insertMemory("parts_current",new Object_Set<std::string>());
    this->_memory->insertMemory("parts_interaction",new Object_Set<std::string>());

    static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_current"))->_set=currentParts;

    for(std::pair<std::string,Object_Worker> w : worker){
        currentActions.insert(std::pair<std::string,AssemblyAction>(w.first,AssemblyAction()));
    }

    while (true) {

        if (status_planner == "replan") {
            for (std::string assembly : static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_current"))->_set) {
                std::cout << assembly << ",";
            }

            sequence = this->_planner->calculateNominalSequence(static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_current"))->_set);

            for (std::pair<std::string, std::list<AssemblyAction> > a : sequence) {
                agent_sequences[a.first].resize(0);
                for (AssemblyAction action : a.second) {
                    std::cout<<a.first<<" executes "<<action._id<<std::endl;
                    agent_sequences[a.first].push_front(action);
                }
            }
            status_planner = "idle";
        }
        bool allDone=true;
        for(std::pair<std::string,Object_Worker> w : worker){

            if(agent_sequences[w.first].size()==0){
                continue;
            }else{
                allDone=false;
            }

            if(this->_agent_threads[w.first]->getStatus()==idle){
                if(this->isApplicable(*agent_sequences[w.first].begin())){
                    currentActions[w.first]=(*agent_sequences[w.first].begin());
                    ROS_INFO("start Task");
                    this->_agent_threads[w.first]->startTask((*agent_sequences[w.first].begin()));
                    if(currentActions[w.first]._type=="hand_over"){
                        this->startInteraction(*agent_sequences[w.first].begin());
                    }
                    //                    this->_agent_threads[w.first]->finishedTask("success");

                }
            }
            if(this->_agent_threads[w.first]->getStatus()==finished){
                this->_agent_threads[w.first]->confirmFinishedTask();
                And_node a=this->_graph->_and_nodes[currentActions[w.first]._id];

                if(currentActions[w.first]._type=="assembly"){
                    static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_current"))->_set=this->joinSubAssemblies(static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_current"))->_set,a.getChildren());
                }
                if(currentActions[w.first]._type=="receive"){
                    this->finishInteraction(*agent_sequences[w.first].begin());
                }

                agent_sequences[w.first].pop_front();
            }
        }

        if(allDone){
            break;
        }
    }
}

bool Assembly_Controller::isApplicable(AssemblyAction action)
{
    if(action._type=="assembly" || action._type=="hand_over"){
        for (std::string part : action._parts) {
            if (static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_current"))->_set.find(part)==static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_current"))->_set.end()) {
                return false;
            }
        }
    }
    if(action._type=="receive"){
        for (std::string part : action._parts) {
            if (static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_interaction"))->_set.find(part)==static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_interaction"))->_set.end()) {
                return false;
            }
        }
    }
    return true;
}

void Assembly_Controller::startInteraction(AssemblyAction a){
    if(a._type!="hand_over"){
        std::cout<<"Error! No interaction starter!"<<std::endl;
        exit(-1);
    }
    for(std::string p : a._parts){
        static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_interaction"))->_set.insert(p);
        static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_current"))->_set.erase(p);
    }
}

void Assembly_Controller::finishInteraction(AssemblyAction a){
    if(a._type!="receive"){
        std::cout<<"Error! No interaction end!"<<std::endl;
        exit(-1);
    }
    for(std::string p : a._parts){
        static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_current"))->_set.insert(p);
        static_cast<Object_Set<std::string>* >(this->_memory->getMemory("parts_interaction"))->_set.erase(p);
    }
}

std::set<std::string> Assembly_Controller::joinSubAssemblies(std::set<std::string> currentParts,std::set<Or_node*> subassemblies){

    std::set<std::string> allparts;
    for(Or_node* o : subassemblies){
        if(currentParts.find(o->getID())==currentParts.end()){
            std::cout<<"Error! Subassembly of action is not in current parts."<<std::endl;
            exit(-1);
        }
        currentParts.erase(o->getID());
        std::set<std::string> parts = this->splitString(o->getID(), "_");
        for (std::string str : parts) {
            if (allparts.find(str) != allparts.end()) {
                std::cout << "Error! In: assembly_controller_joinSubAssemblies, part is already in set." << std::endl;
                exit(-1);
            }
            allparts.insert(str);
        }

    }

    std::string joinedPart;
    for (std::string p : allparts) {
        joinedPart.append(p);
        joinedPart.append("_");
    }
    joinedPart.pop_back();
    currentParts.insert(joinedPart);
    return currentParts;
}

std::set<std::string> Assembly_Controller::splitString(std::string str, std::string delimiter)
{
    std::set<std::string> parts;

    if (str.find(delimiter)==std::string::npos) {
        parts.insert(str);
        return parts;
    }

    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        parts.insert(token);
        str.erase(0, pos + delimiter.length());
    }
    parts.insert(str);
    return parts;
}

void Assembly_Controller::setupController(Memory *mem, ANDORgraph *graph, std::set<Object_Worker> worker){

    this->_planner=new Planner();
    this->_memory=mem;
    this->_planner->linkToMemory(this->_memory);
    this->_graph=graph;

    std::set<Object_Assembly> assemblies=this->_graph->getAssemblies();

    this->setupAssemblies(assemblies);
    this->setupWorker(worker);
    this->setupAndOrGraph(graph);

}

void Assembly_Controller::setSkills(std::map<std::string, std::set<std::string> > skills){
    this->_skills=skills;
    this->_planner->_skills=this->_skills;
}

void Assembly_Controller::resetGraph(ANDORgraph *graph){
    this->_graph=graph;

    std::set<Object_Assembly> assemblies=this->_graph->getAssemblies();

    this->setupAssemblies(assemblies);
    this->setupAndOrGraph(graph);
}

void Assembly_Controller::setupAssemblies(std::set<Object_Assembly> assemblies){

    this->_memory->insertMemory("map_assemblies",new Object_Map<Object_Assembly>());
    for(Object_Assembly assembly : assemblies){
        static_cast<Object_Map<Object_Assembly>* >(this->_memory->getMemory("map_assemblies"))->_map.insert(std::pair<std::string,Object_Assembly>(assembly.getAssemblyName(),assembly));
    }
}

void Assembly_Controller::setupWorker(std::set<Object_Worker> worker){

    this->_memory->insertMemory("map_worker",new Object_Map<Object_Worker>());

    // save workers to memory
    for(Object_Worker w : worker){
        static_cast<Object_Map<Object_Worker>* >(this->_memory->getMemory("map_worker"))->_map.insert(std::pair<std::string,Object_Worker>(w.getId(),w));
    }


    // create thread for every worker
    for (Object_Worker w : worker) {
        Thread_Agent* t = new Thread_Agent(&w);
        this->_agent_threads.insert(std::pair<std::string,Thread_Agent*>(w.getId(),t));
    }
}

void Assembly_Controller::setupAndOrGraph(ANDORgraph *graph){
    std::map<std::string,Object_Worker> m=static_cast<Object_Map<Object_Worker> *>(this->_memory->getMemory("map_worker"))->_map;
    this->_graph=graph;
    this->_graph->setupGraph(&static_cast<Object_Map<Object_Assembly>* >(this->_memory->getMemory("map_assemblies"))->_map);
    if(!this->_graph->checkAssembly(m)){
        ROS_ERROR("Error in And-Or-graph");
        exit(0);
    }

    this->_memory->insertMemory("andorgraph",new Object_ANDORgraph(this->_graph));
}

void Assembly_Controller::setupStandardInteractionCosts(double val){

    std::map<std::string,Object_Worker> m1=static_cast<Object_Map<Object_Worker> *>(this->_memory->getMemory("map_worker"))->_map;

    Object_ANDORgraph* graph = static_cast<Object_ANDORgraph*>(this->_memory->getMemory("andorgraph"));

    std::map<std::string,double> costMap_tmp;
    for(std::pair<std::string,Object_Worker> worker : m1){
        for(Or_node* leaf : graph->_graph->_leafs){
            std::string name = "h_"+worker.first+"_"+leaf->getID();
            costMap_tmp.insert(std::pair<std::string,double>(name,val));
        }
    }
    std::map<std::pair<std::string,std::string>,double>* m2=&static_cast<Object_CostMap*>(this->_memory->getMemory("map_cost"))->_costmap;
    std::map<std::string,Object_Worker> m3=static_cast<Object_Map<Object_Worker> *>(this->_memory->getMemory("map_worker"))->_map;
    for(std::pair<std::string,double> cost : costMap_tmp){
        for(std::pair<std::string,Object_Worker> worker : m3){
            m2->insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>(worker.first,cost.first),cost.second));
        }
    }
}

std::map<std::string,Thread_Agent*>* Assembly_Controller::getAgents(){
    return &this->_agent_threads;
}
