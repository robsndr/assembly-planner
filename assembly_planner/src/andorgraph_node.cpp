#include "assembly_planner/andorgraph_node.h"

AndOrGraph_node::AndOrGraph_node(std::set<Or_node*> or_nodes, Memory* memory, std::map<std::string, std::set<std::string> > skills){
    this->_memory=memory;
    this->_or_nodes=or_nodes;
    this->_skills=skills;
    this->_and_nodes.clear();
    this->_assignmentMap_worker_to_actions.clear();

    this->_graph=static_cast<Object_ANDORgraph*>(this->_memory->getMemory("andorgraph"))->_graph;

    this->_g=0;
    this->_h=0;
    this->_f=0;

	this->_worker = this->getWorker();

    // setup empty sequences and durations for every worker

    // get children from OR nodes and insert them as AND nodes
    for(Or_node* or_node : this->_or_nodes){
        for(And_node* and_node : or_node->getChildren()){
            this->_and_nodes.insert(and_node);
        }
    }
}

AndOrGraph_node::AndOrGraph_node(Node_Data data){

    // prepare data structures
    this->_or_nodes.clear();
    this->_and_nodes.clear();
    this->_actions.clear();
    this->_assignmentMap_worker_to_actions=data.assignmentMap_worker2action;

    // set pointer to memory module
    this->_memory=data.memory;
    this->_graph=static_cast<Object_ANDORgraph*>(this->_memory->getMemory("andorgraph"))->_graph;
    this->_skills=data.skills;
//    this->_skills=static_cast<Object_Map<std::set<std::string> > >(this->_memory->getMemory("skills"))->_map;

    this->_g=0;
    this->_h=0;
    this->_f=0;

    this->_sequence = data.sequence;


    // copy all current AND nodes
    this->_and_nodes=data.and_nodes;

	this->_worker = this->getWorker();

    // add all OR leaf nodes - state construction
    for(Or_node* leaf : data.leafs){
        this->_or_nodes.insert(leaf);
    }

    for(And_node* and_node : this->_and_nodes){
        this->_or_nodes.insert(and_node->getParent());
    }

    // iterate over all workers and their assignments to actions
    for(std::pair<std::string,std::string> mapping_worker_to_action : this->_assignmentMap_worker_to_actions){
        if(mapping_worker_to_action.second=="none"){

            // continue if no action is assigned to this worker
            continue;
        }
        if(this->_worker[mapping_worker_to_action.first]->isCapable(this->_skills[mapping_worker_to_action.second])){
            // continue if worker has not the necessary skill to perform this action
//            std::cout<<"no skilL"<<std::endl;
//            continue;
        }
        // check if worker has been assigned to interaction but can not fullfil the requirements
        // in this case delete the assignment and re-add the OR node that is the parent of the interaction
        if(mapping_worker_to_action.second[0]=='h'){
            Or_node* tmp_node=*this->_graph->_and_nodes[mapping_worker_to_action.second].getChildren().begin();
            if(!this->_worker[mapping_worker_to_action.first]->isReachable(tmp_node->getAssembly())){
                this->_or_nodes.insert(this->_graph->_and_nodes[mapping_worker_to_action.second].getParent());
                this->_assignmentMap_worker_to_actions[mapping_worker_to_action.first]="none";
                continue;
            }
        }

		std::set<std::string> parts;
		bool wasInt = false;
        for (Or_node* or_node : this->_graph->_and_nodes[mapping_worker_to_action.second].getChildren()) {
			parts.insert(or_node->getID());
            if (or_node->isLocal()) {
				wasInt = true;
			}
		}
		AssemblyAction a;
		if (mapping_worker_to_action.second[0]=='h') {
            a = AssemblyAction("hand_over", { "w1",mapping_worker_to_action.first }, parts,mapping_worker_to_action.second);
		}
		else {
            a = AssemblyAction("assembly", { mapping_worker_to_action.first }, parts,mapping_worker_to_action.second);
		}
		
	
		this->_sequence[mapping_worker_to_action.first].push_back(a);


        // add assigned AND node to actions (not necessary)
        this->_actions.insert(mapping_worker_to_action.second);
        // delete assigned action from set of AND nodes

        this->_or_nodes.erase(this->_graph->_and_nodes[mapping_worker_to_action.second].getParent());
        this->_and_nodes.erase(&(this->_graph->_and_nodes[mapping_worker_to_action.second]));

        // iterate over all the partitions resulting from assigned action
        for(Or_node* partition : this->_graph->_and_nodes[mapping_worker_to_action.second].getChildren()){
            // check if part is reachable
            if(this->_worker[mapping_worker_to_action.first]->isReachable(partition->getAssembly())){
                // if part is reachable add it to OR nodes
                this->_or_nodes.insert(partition);
                for(And_node* and_node : partition->getChildren()){
                    this->_and_nodes.insert(and_node);
                }
            }else{
                if(mapping_worker_to_action.second[0]=='h'){
                    continue;
                }
                // if part is not reachable create new intermediate AND node (interaction) and OR node

                //                std::cout<<"not reachable: ("<<mapping_worker_to_action.first<<","<<partition->getAssembly()->getAssemblyString()<<")"<<std::endl;
                //                std::cout<<"(x,y)=("<<partition->getAssembly()->getX()<<","<<partition->getAssembly()->getY()<<")"<<std::endl;

                Or_node or_node_temp(partition->getAssembly(),true);

                // create a unique name for the interaction
                std::string interaction_name="h_"+mapping_worker_to_action.first+"_"+partition->getID();

                And_node interaction_node(interaction_name);
                // set OR node as child which represents the assembly that could not be reached by this worker
                std::set<Or_node*> tmp_set={partition};
                interaction_node.setChildren(tmp_set);

                // add both nodes to graph
                this->_graph->_or_nodes.insert(std::pair<std::string,Or_node>(or_node_temp.getID(),or_node_temp));
                this->_graph->_and_nodes.insert(std::pair<std::string,And_node>(interaction_name,interaction_node));

                this->_graph->_or_nodes[or_node_temp.getID()].setChildren({&this->_graph->_and_nodes[interaction_name]});
                this->_graph->_and_nodes[interaction_name].setParent(&this->_graph->_or_nodes[or_node_temp.getID()]);
                // add interaction node the the AND nodes
                this->_and_nodes.insert(&this->_graph->_and_nodes[interaction_name]);
                // add intermediate OR node the the OR nodes
                this->_or_nodes.insert(&this->_graph->_or_nodes[or_node_temp.getID()]);
                // assign this worker to the interaction
                //this->_interaction_set.insert(std::pair<std::string,std::string>(interaction_name,mapping_worker_to_action.first));

				std::set<std::string> parts;
				parts.insert(partition->getAssembly()->getAssemblyName());

                AssemblyAction a("receive", { mapping_worker_to_action.first,"w2" }, parts,mapping_worker_to_action.second);

				this->_sequence[mapping_worker_to_action.first].push_back(a);

            }
        }
    }
}

AndOrGraph_node::~AndOrGraph_node()
{

}

bool AndOrGraph_node::equalsNode(AStar_node* node){
    return this->getUniqueString()==node->getUniqueString();
}

std::list<algorithms::search::AStar_node*> AndOrGraph_node::getNeighbors(){
    std::list<AStar_node*> neighbors(0);

    std::set<Or_node*> leaf_nodes;
    leaf_nodes.clear();
    unsigned nAndNodes=0;

    // get the number of possible combinations of AND nodes and count AND nodes and collect leaf nodes (OR nodes without children)
    unsigned prod_or=1;
    unsigned cntNonLeafs=0;

    //    std::cout<<"OR nodes: ";
    for(Or_node* or_node : this->_or_nodes){
        //        std::cout<<or_node->getID();
        if(this->_graph->_leafs.find(or_node)!=this->_graph->_leafs.end()){
            leaf_nodes.insert(or_node);
        }else{
            prod_or*=or_node->getChildren().size();
            nAndNodes+=or_node->getChildren().size();
            cntNonLeafs++;
        }
    }
    //    std::cout<<std::endl;

    // if there are no actions available return an empty list of neighbors
    if(nAndNodes==0){
        return neighbors;
    }

    std::vector<std::vector<std::string> > combs;
    std::vector<std::string> current2;
    std::set<Or_node*>::iterator it_or=this->_or_nodes.begin();

    // get all possible AND nodes
    pickActions(combs,current2,it_or);

    // get all possible combinations of AND nodes
    std::vector<std::vector<unsigned> > assignment_actions(prod_or);
    for(unsigned i=0;i<assignment_actions.size();i++){
        assignment_actions[i].resize(this->_or_nodes.size());
    }
    std::vector<std::vector<std::string> > assignment_actions_test;
    for(unsigned i=0;i<combs.size();i++){
        if(combs[i].size()==cntNonLeafs){
            assignment_actions_test.push_back(combs[i]);
        }
    }



    std::vector<std::vector<std::string> > combinations_worker;
    std::vector<std::string> current(0);
    std::vector<std::string> input;
    std::set<std::string> current_worker;

    for(std::pair<std::string,Object_Worker*> worker : this->_worker){
        input.push_back(worker.first);
    }


    std::set<std::map<std::string,std::string> > assignmentMap_set;

    std::set<And_node*> and_nodes_exp;
    unsigned p_tmp;

    // iterate through every possible combination of actions
    for(unsigned i=0;i<assignment_actions_test.size();i++){
        if(assignment_actions_test[i].size()!=cntNonLeafs){
            continue;
        }
        and_nodes_exp.clear();
        for(unsigned j=0;j<assignment_actions_test[i].size();j++){
            and_nodes_exp.insert(&this->_graph->_and_nodes[assignment_actions_test[i][j]]);
        }

        // iterate through every possible combination of workers [1 to min(number of available actions,number of workers)] for a particular set of actions
        unsigned p_max;
        if(and_nodes_exp.size()>this->_worker.size()){
            p_max=this->_worker.size();
        }else{
            p_max=and_nodes_exp.size();
        }
        for(unsigned p=1;p<=p_max;p++){
            combinations_worker.resize(0);
            current.resize(0);
            //            if(p>this->_data->_worker.size()){
            //                p_tmp=this->_data->_worker.size();
            //            }else{
            //                p_tmp=p;
            //            }
            math_functions::nchoosek(0,p,combinations_worker,current,input);

            // iterate through all possible assignements for particular set of workers
            for(unsigned q=0;q<combinations_worker.size();q++){

                current_worker.clear();
                for(unsigned i=0;i<combinations_worker[q].size();i++){
                    current_worker.insert(combinations_worker[q][i]);
                }


                long long n=math_functions::factorial(and_nodes_exp.size())/(math_functions::factorial(and_nodes_exp.size()-current_worker.size()));
                std::vector<std::vector<unsigned> > assignment_workers(n);
                for(unsigned i=0;i<assignment_workers.size();i++){
                    assignment_workers[i].resize(current_worker.size());
                }

                std::vector<bool> assigned;
                assigned.assign(and_nodes_exp.size(),false);

                assignAgents(assignment_workers,assigned,0,true,0,and_nodes_exp.size(),current_worker.size());



                for(unsigned l=0;l<assignment_workers.size();l++){
                    unsigned m=0;
                    std::map<std::string,std::string> assignmentMap_worker_to_actions;
                    // find all workers not assigned to an action
                    for(std::pair<std::string,Object_Worker*> worker : this->_worker){
                        if(current_worker.find(worker.first)==current_worker.end()){
                            assignmentMap_worker_to_actions[worker.first]="none";
                        }
                    }
                    for(std::string worker : current_worker){
                        unsigned o=0;
                        for(And_node* and_node : and_nodes_exp){
                            if(o==assignment_workers[l][m]){
                                assignmentMap_worker_to_actions.insert(std::pair<std::string,std::string>(worker,and_node->getID()));
                            }
                            o++;
                        }
                        m++;
                    }

                    // check for double assignments
                    bool alreadyAssigned=false;
                    for(std::map<std::string,std::string> map : assignmentMap_set){
                        if(map==assignmentMap_worker_to_actions){
                            alreadyAssigned=true;
                        }
                    }
                    assignmentMap_set.insert(assignmentMap_worker_to_actions);
                    if(alreadyAssigned){
                        continue;
                    }
                    Node_Data data;
                    data.and_nodes=and_nodes_exp;
                    data.assignmentMap_worker2action=assignmentMap_worker_to_actions;
                    data.leafs=leaf_nodes;
                    data.memory=this->_memory;
                    data.sequence=this->_sequence;
                    data.skills=this->_skills;
                    neighbors.push_back(new AndOrGraph_node(data));
                }

            }
        }
    }
    if(neighbors.size()==2){
        int sad=3;
    }

    // plausiblity checks



//    std::cout<<"NodeCreated "<<neighbors.size()<<" neighbors."<<std::endl;
    return neighbors;
}

std::string AndOrGraph_node::getUniqueString(){

    std::string unique_str;
    unique_str.append("{");
    for(Or_node* or_node : this->_or_nodes){
        unique_str.append(or_node->getID());
        unique_str.append(",");
    }
    unique_str.pop_back();
    unique_str.append("}");
//    unique_str.append(",");
//    unique_str.append("{");
//    for(std::pair<std::string,std::string> assignemnt : this->_assignmentMap_worker_to_actions){
//        unique_str.append("(");
//        unique_str.append(assignemnt.first);
//        unique_str.append(",");
//        unique_str.append(assignemnt.second);
//        unique_str.append(")");
//        unique_str.append(",");
//    }


////    for(std::pair<std::string,std::list<std::string> > seq : this->_sequence){
////        unique_str.append("{");
////        unique_str.append(seq.first);
////        unique_str.append("->");
////        unique_str.append("[");
////        for(std::string str : seq.second){
////            unique_str.append(str);
////            unique_str.append(",");
////        }
////        unique_str.pop_back();
////        unique_str.append("]");
////        unique_str.append("}");
////        unique_str.append(",");
////    }
//    unique_str.pop_back();
//    unique_str.append("}");
    return unique_str;
}

bool AndOrGraph_node::goalTest(AStar_node* node2){
    AndOrGraph_node* node = static_cast<AndOrGraph_node*>(node2);

    std::string unique_str;
    unique_str.append("{");
    for(Or_node* or_node : this->_or_nodes){
        unique_str.append(or_node->getID());
        unique_str.append(",");
    }
    unique_str.pop_back();
    unique_str.append("}");

    std::string unique_str2;
    unique_str2.append("{");
    for(Or_node* or_node : *node->getOrNodes()){
        unique_str2.append(or_node->getID());
        unique_str2.append(",");
    }
    unique_str2.pop_back();
    unique_str2.append("}");

    return unique_str==unique_str2;
}

double AndOrGraph_node::getActionCost(AStar_node *node){

    Object_CostMap* cm= static_cast<Object_CostMap*>(this->_memory->getMemory("map_cost"));
    std::map<std::pair<std::string,std::string>,double> cost_map=static_cast<Object_CostMap*>(this->_memory->getMemory("map_cost"))->_costmap;

    
    double c=0;
    for(std::pair<std::string,std::string> w : this->_assignmentMap_worker_to_actions){
        c+=cost_map[std::pair<std::string,std::string>(w.first,w.second)];
    }
    std::cout<<"Cost: "<<c<<std::endl;
    return c;

//    AndOrGraph_node* andor_node = static_cast<AndOrGraph_node*>(node);
//
//    std::map<std::string,double> duration_pre=andor_node->getDuration();
//
//    std::map<std::string,double> duration;
//    for(std::pair<std::string,Object_Worker*> worker : this->_worker){
//        duration.insert(std::pair<std::string,double>(worker.first,0));
//    }
//
//    this->_total_duration=duration_pre;
//
//    // accumulate the cost for every agent
//    for(std::pair<std::string,Object_Worker*> worker : this->_worker){
//        // // check if worker has been assigned to an action
//        if(_assignmentMap_worker_to_actions[worker.second->getID()]!="none"){
//            duration[worker.second->getID()]+=this->_costMap[std::make_pair(worker.second->getID(), this->_data->_costMap_worker_to_action[worker.second->getID()][this->_assignmentMap_worker_to_actions[worker.second->getID()]];
//            this->_total_duration[worker.second->getID()]+=this->_data->_costMap_worker_to_action[worker.second->getID()][this->_assignmentMap_worker_to_actions[worker.second->getID()]];
//        }
//    }
//    for(std::pair<std::string,std::string> inter : this->_interaction_set){
//        this->_sequence[inter.second].push_back(inter.first);
//        this->_total_duration[inter.second]+=this->_data->_costMap_worker_to_action[inter.second][inter.first];
//        duration[inter.second]+=this->_data->_costMap_worker_to_action[inter.second][inter.first];
//    }
//
//    double sum1=0;
//    double max1=0;
//    std::string maxWorker;
//    for(std::pair<std::string,double> dur : this->_total_duration){
//        sum1+=dur.second;
//        if(dur.second>max1){
//            max1=dur.second;
//            maxWorker=dur.first;
//        }
//    }
//    double sum2=0;
//    double max2=0;
//    for(std::pair<std::string,double> dur : duration_pre){
//        sum2+=dur.second;
//        if(dur.second>max2){
//            max2=dur.second;
//        }
//    }
//    double max3=0;
//    for(std::pair<std::string,double> dur : duration){
//        if(dur.second>max3){
//            max3=dur.second;
//        }
//    }
////    this->_actionCost=max1-duration_pre[maxWorker];
//    this->_actionCost=max3;
//
////    this->_actionCost=(sum1-sum2)/this->_total_duration.size();
//    return this->_actionCost;
	return 0;
}

double AndOrGraph_node::heuristic(AStar_node *node){
    unsigned maxChildren=0;
    for(Or_node* or_node : this->_or_nodes){
        if(or_node->getAssembly()->getPartNames()->size()-1>maxChildren){
            maxChildren=or_node->getAssembly()->getPartNames()->size()-1;
        }
    }
    if(maxChildren==0){
//        return this->_actionCost;
        return 0;
    }
//    std::cout<<"heuristic: "<<ceil(ceil(log2(maxChildren))*this->_data->getMinCosts()/this->_data->_worker.size())+this->_actionCost<<std::endl;
//    std::cout<<"min cost: "<<this->_data->getMinCosts()<<std::endl;
    return ceil(ceil(log2(maxChildren))/this->_worker.size());
//    return ceil(ceil(log2(maxChildren))*this->_data->getMinCosts()/this->_data->_worker.size());
//        return 0;
}

std::set<And_node*>* AndOrGraph_node::getAndNodes(){
    return &this->_and_nodes;
}

std::set<Or_node *> *AndOrGraph_node::getOrNodes(){
    return &this->_or_nodes;
}

//std::map<std::string,double> AndOrGraph_node::getDuration(){
//    return this->_total_duration;
//}

std::map<std::string,std::string> AndOrGraph_node::getMappingWorkerToActions(){
    return this->_assignmentMap_worker_to_actions;
}

std::map<std::string,std::list<AssemblyAction> > AndOrGraph_node::getActionSequence(){
    return this->_sequence;
}

// DOES NOT WORK PROPERLY //
std::set<Or_node*>::iterator AndOrGraph_node::pickActions(std::vector<std::vector<std::string> >& combs,std::vector<std::string>& current, std::set<Or_node*>::iterator it_or){

    if(it_or==this->_or_nodes.end()){
        --it_or;
        return it_or;
    }
    if(this->_graph->_leafs.find(*it_or)!=this->_graph->_leafs.end()){
        ++it_or;
        it_or=pickActions(combs,current,it_or);
        return it_or;
    }

    for(And_node* and_node : (*it_or)->getChildren()){
        current.push_back(and_node->getID());
        ++it_or;

        it_or=pickActions(combs,current,it_or);
        combs.push_back(current);
        current.pop_back();
    }
    --it_or;
    return it_or;
}

std::map<std::string,Object_Worker*> AndOrGraph_node::getWorker()
{
    std::map<std::string,Object_Worker>* m=&static_cast<Object_Map<Object_Worker> *>(this->_memory->getMemory("map_worker"))->_map;
    std::map<std::string,Object_Worker*> worker;
    for(std::map<std::string,Object_Worker>::iterator it=m->begin();it!=m->end();++it){
        worker.insert(std::pair<std::string,Object_Worker*>((*it).first,&(*it).second));
    }
    return worker;
}

double AndOrGraph_node::getCost(std::string type_worker, std::string action)
{
    std::string type_action = this->_graph->_types_actions[action];
    Object_Type_Action* map = static_cast<Object_Type_Action*>(this->_memory->getMemory(type_action));
	return map->getCost(type_worker);
}

unsigned AndOrGraph_node::assignAgents(std::vector<std::vector<unsigned> >& assignment_global,std::vector<bool>& assigned, unsigned agent,bool newAssignment,unsigned assignment_cnt,unsigned nAndnodes,unsigned nWorker){
    unsigned assignment_cnt_new=assignment_cnt;
    bool isNew=newAssignment;
    for(unsigned i=0;i<nAndnodes;i++){
        if(assigned[i]){
            continue;
        }
        if(!isNew){
            assignment_global[assignment_cnt_new+1]=assignment_global[assignment_cnt_new];
            assignment_cnt_new++;
            for(unsigned j=agent;j<nWorker;j++){
                assigned[assignment_global[assignment_cnt_new][j]]=false;
            }
        }
        isNew=false;

        assignment_global[assignment_cnt_new][agent]=i;

        assigned[i]=true;
        if(agent<nWorker-1){
            assignment_cnt_new=assignAgents(assignment_global,assigned,agent+1,true,assignment_cnt_new,nAndnodes,nWorker);
            for(unsigned j=agent;j<nWorker;j++){
                assigned[assignment_global[assignment_cnt_new][j]]=false;
            }
        }

    }
    return assignment_cnt_new;
}
