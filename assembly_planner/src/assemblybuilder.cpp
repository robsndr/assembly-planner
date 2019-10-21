#include "assembly_planner/assemblybuilder.h"

AssemblyBuilder::AssemblyBuilder()
{

}

AssemblyBuilder::AssemblyBuilder(std::set<Object_Assembly> assemblies, std::set<Object_Worker> worker, std::map<std::string, std::map<std::string, double> > costs,Memory* mem,Planer* plan){
    this->_memory=mem;
    this->_planner=plan;
    for(Object_Assembly assembly : assemblies){
        int d=this->_memory->memory_factual.size();
        Object_Map<Object_Assembly>*m= static_cast<Object_Map<Object_Assembly>* >(this->_memory->getMemory("map_assemblies"));
        static_cast<Object_Map<Object_Assembly>* >(this->_memory->getMemory("map_assemblies"))->_map.insert(std::pair<std::string,Object_Assembly>(assembly.getAssemblyName(),assembly));
        //        this->_data._assemblies_all.insert(std::pair<std::string,Object_Assembly>(assembly.getAssemblyName(),assembly));
    }
    for(Object_Worker w : worker){
        static_cast<Object_Map<Object_Worker>* >(this->_memory->getMemory("map_worker"))->_map.insert(std::pair<std::string,Object_Worker>(w.getId(),w));
    }
    //    this->_data._costMap_worker_to_action=costs;
}

AssemblyBuilder::~AssemblyBuilder(){

}

void AssemblyBuilder::applyAction(std::string action){

    for(Or_node* or_node : this->_planner->getGraph()->_and_nodes[action].getChildren()){
        this->_currentParts.erase(or_node->getAssembly()->getAssemblyName());
    }

    this->_currentParts.insert(std::pair<std::string,Object_Assembly*>(this->_planner->getGraph()->_and_nodes[action].getParent()->getAssembly()->getAssemblyName(),this->_planner->getGraph()->_and_nodes[action].getParent()->getAssembly()));
}

std::string AssemblyBuilder::joinAssemblies(std::set<std::string> parts) {
    std::set<std::string> newPart;

    // TODO: extract single part names from variable: parts, then put them back together alphabetically
    return "test";
}

void AssemblyBuilder::connectToMemory(Memory *mem){
    this->_memory=mem;
}

bool AssemblyBuilder::isApplicable(std::string action)
{
    return false;
}



std::map<std::string,Object_Assembly*> AssemblyBuilder::getLeafAssemblies(){

    std::map<std::string, Object_Assembly*> assemblyConfig;

    for(Or_node* leaf : this->_planner->getGraph()->_leafs){
        assemblyConfig.insert(std::pair<std::string,Object_Assembly*>(leaf->getID(),leaf->getAssembly()));
    }
    return assemblyConfig;
}

void AssemblyBuilder::setANDORgraph(ANDORgraph *graph){
    if(!checkAssembly(graph)){
        std::cout<<"ERROR! Assembly is not valid!"<<std::endl;
        exit(0);
    }
    this->_planner->setAndOrGraph(graph);
    setupStandardInteractionCosts(8);
    this->_currentParts=getLeafAssemblies();
}

void AssemblyBuilder::setCurrentConfiguration(std::map<std::string, Object_Assembly *> currentConf){
    this->_currentParts=currentConf;

}

ANDORgraph* AssemblyBuilder::getANDORgraph(){
    return this->_planner->getGraph();
}

//DataStorage* AssemblyBuilder::getDataStorage(){
//    return &this->_data;
//}

std::map<std::string, Object_Assembly *> AssemblyBuilder::getAssemblies(){
    std::map<std::string,Object_Assembly*> assemblies;
    //    for(std::pair<std::string,Assembly> assembly : this->_data._assemblies_all){
    std::map<std::string,Object_Assembly>* m=&static_cast<Object_Map<Object_Assembly> *>(this->_memory->getMemory("map_assemblies"))->_map;
    for(std::map<std::string,Object_Assembly>::iterator it=m->begin();it!=m->end();++it){
        assemblies.insert(std::pair<std::string,Object_Assembly*>((*it).first,&(*it).second));
    }
    return assemblies;
}

std::map<std::string,Object_Assembly*> AssemblyBuilder::getCurrentConfiguration(){
    return this->_currentParts;
}

void AssemblyBuilder::setupStandardInteractionCosts(double val){

    std::map<std::string,Object_Worker> m1=static_cast<Object_Map<Object_Worker> *>(this->_memory->getMemory("map_worker"))->_map;

    std::map<std::string,double> costMap_tmp;
    for(std::pair<std::string,Object_Worker> worker : m1){
        for(Or_node* leaf : this->_planner->getGraph()->_leafs){
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

std::map<std::string, std::list<AssemblyAction> > AssemblyBuilder::calculateNominalSequence(std::map<std::string,Object_Assembly*> currentSubassemblies,std::map<std::string,AssemblyAction> appliedActions_in){

    // get the pointers to the OR nodes of the current assembly status
    //    std::cout<<"listing current assemblies: "<<std::endl;
    std::set<Or_node*> currentOrNodes;
    for(std::pair<std::string,Object_Assembly*> assembly : currentSubassemblies){
        std::string name_part=assembly.second->getAssemblyName();
        currentOrNodes.insert(&this->_planner->getGraph()->_or_nodes[name_part]);
    }

    ////    this->_data._graph->setLeafs(currentOrNodes);

    // setup the start node
    AndOrGraph_node start({{this->_planner->getGraph()->_root}},this->_memory,this->_planner);
    start.setPredecessor(NULL);
    // setup the goal node with the current assembly status
    AndOrGraph_node goal(currentOrNodes,this->_memory,this->_planner);

    // conduct the search
    std::cout<<"astar: seaching...";
    AndOrGraph_node* node = static_cast<AndOrGraph_node*>(this->_planner->getAstar()->search(&start,&goal));
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

    // get all actions that need to be applied
    std::map<std::string,AssemblyAction> appliedActions_d=appliedActions_in;
    std::map<std::string,AssemblyAction> appliedActions=appliedActions_in;
    std::map<std::string,std::string> appliedActions_tmp;

    
//    for(std::pair<std::string,std::list<AssemblyAction> > worker : action_sequence){
//        for(AssemblyAction action : worker.second){
//            if(action._type=="idle"){
//                continue;
//            }
//            appliedActions_d.insert(action);
//        }
//    }
    //    std::map<std::string,double> duration=node->getDuration();
    //    for(std::pair<std::string,std::list<std::string> > worker : action_sequence){
    //        std::cout<<"Worker "<<worker.first<<": ";
    //        for(std::string action : worker.second){
    //            if(action=="none"){
    //                continue;
    //            }
    //            std::cout<<action<<", ";
    //        }
    //        std::cout<<" duration: "<<duration[worker.first]<<std::endl;
    //    }

    //    while(appliedActions!=appliedActions_d){
    //        for(std::pair<std::string,std::list<std::string> > worker : action_sequence){
    //            for(std::string action : worker.second){
    //                if(action=="none" || action[0]=='h'){
    //                    continue;
    //                }
    //                if(appliedActions.find(action)==appliedActions.end() && std::includes(appliedActions.begin(),appliedActions.end(),this->_planner->getGraph()->_order_actions[action].begin(),this->_planner->getGraph()->_order_actions[action].end())){
    //                    appliedActions_tmp.insert(std::pair<std::string,std::string>(worker.first,action));
    //                }
    //            }
    //        }

    //        actions_sequence.push_back(std::map<std::string,std::string>());
    //        for(std::pair<std::string,std::string> action : appliedActions_tmp){
    //            actions_sequence.back().insert(std::pair<std::string,std::string>(action.first,action.second));
    //            appliedActions.insert(action.second);
    //        }
    //        appliedActions_tmp.clear();
    //        if(actions_sequence.back().size()==0){
    //            std::cout<<"Error! Could not apply any actions!"<<std::endl;
    //            break;
    //        }
    //    }

    //    for(std::map<std::string,std::string> elem : actions_sequence){
    //        for(std::pair<std::string,std::string> action : elem){
    //            std::cout<<action.second<<" by "<<action.first<<", ";
    //        }
    //        std::cout<<std::endl;
    //    }

    return action_sequence;
}

bool AssemblyBuilder::checkAssembly(ANDORgraph *assembly){

    std::cout<<"---------"<<std::endl;
    std::cout<<"---------"<<std::endl;
    std::cout<<"Output of assembly"<<std::endl;
    std::cout<<"Root node: "<<assembly->_root->getID()<<std::endl;
    std::cout<<"---------"<<std::endl;
    std::cout<<"OR nodes:"<<std::endl;
    std::cout<<"number of OR nodes: "<<assembly->_or_nodes.size()<<std::endl;
    for(std::pair<std::string,Or_node> or_node : assembly->_or_nodes){
        std::cout<<"id: "<<or_node.second.getID()<<", children: ";
        for(And_node* and_node : or_node.second.getChildren()){
            std::cout<<and_node->getID()<<",";
        }
        std::cout<<std::endl;
    }
    std::cout<<"---------"<<std::endl;
    std::cout<<"leafs:"<<std::endl;
    for(Or_node* leaf : assembly->_leafs){
        std::cout<<leaf->getID()<<",";
    }
    std::cout<<std::endl;
    std::cout<<"---------"<<std::endl;
    std::cout<<"AND nodes:"<<std::endl;
    std::cout<<"number of AND nodes: "<<assembly->_and_nodes.size()<<std::endl;
    for(std::pair<std::string,And_node> and_node : assembly->_and_nodes){
        std::cout<<"id: "<<and_node.second.getID()<<", children: ";
        for(Or_node* or_node : and_node.second.getChildren()){
            std::cout<<or_node->getID()<<",";
        }
        std::cout<<std::endl;
        std::cout<<"parent: "<<and_node.second.getParent()->getID()<<std::endl;
    }
    std::cout<<"---------"<<std::endl;
    std::cout<<"Action relations"<<std::endl;
    for(std::pair<std::string,std::set<std::string> > action : assembly->_order_actions){
        std::cout<<"action: "<<action.first<<std::endl;
        std::cout<<"conditional actions: ";
        for(std::string preAction : action.second){
            std::cout<<preAction<<",";
        }
        std::cout<<std::endl;
    }

    for(Or_node* part : assembly->_leafs){
        bool isReachable=false;
        std::map<std::string,Object_Worker> m=static_cast<Object_Map<Object_Worker> *>(this->_memory->getMemory("map_worker"))->_map;
        for(std::pair<std::string,Object_Worker> worker : m){
            if(worker.second.isReachable(part->getAssembly())){
                isReachable=true;
                break;
            }
        }
        if(!isReachable){
            std::cout<<"Assembly error! Not all parts are reachable by the workers!"<<std::endl;
            return false;
        }
    }
    return true;

}
