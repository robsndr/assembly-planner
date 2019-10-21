#include "assembly_planner/assemblies.h"

SimpleMonotoneAssembly::SimpleMonotoneAssembly(){

}

SimpleMonotoneAssembly::SimpleMonotoneAssembly(std::list<Object_Assembly*> parts, std::map<std::string, Object_Assembly> *assemblies){

    std::list<std::pair<std::string,Or_node> > tmp_list;
    std::set<std::string> tmp_set;
    std::string name_subassembly;
    std::string tmp_child;

    unsigned i=0;
    for(Object_Assembly* part : parts){
        std::string name_part=part->getAssemblyName();
        tmp_list.push_back(std::pair<std::string,Or_node>(name_part,Or_node(part)));
        this->_or_nodes.insert(std::pair<std::string,Or_node>(name_part,Or_node(part)));

        this->_leafs.insert(&this->_or_nodes[name_part]);
        unsigned j=1;
        tmp_set.insert(part->getAssemblyName());
        Object_Assembly tmp_assembly(tmp_set);

        if(i==0){
            name_subassembly=part->getAssemblyName();
        }else{
            name_subassembly+="_"+part->getAssemblyName();
        }

        if(tmp_list.size()>1){
            this->_or_nodes.insert(std::pair<std::string,Or_node>(name_subassembly,Or_node(&assemblies->operator[](name_subassembly))));

            if(tmp_list.size()==parts.size()){
                this->_root=&this->_or_nodes[name_subassembly];
            }

            std::string name_action="a"+boost::lexical_cast<std::string>(i);
            this->_and_nodes.insert(std::pair<std::string,And_node>(name_action,And_node(name_action)));
            std::set<Or_node*> childNodes={{&this->_or_nodes[name_part]},{&this->_or_nodes[tmp_child]}};
            this->_and_nodes[name_action].setChildren(childNodes);

            this->_and_nodes[name_action].setParent(&this->_or_nodes[name_subassembly]);
            this->_or_nodes[name_subassembly].setChildren({&this->_and_nodes[name_action]});

            std::string preName;
            if(i==1){
                preName="none";
            }else{
                preName="a"+boost::lexical_cast<std::string>(i-1);
            }
            std::set<std::string> preActions={preName};

            this->_order_actions[name_action]=preActions;

        }
        tmp_child=tmp_assembly.getAssemblyName();
        i++;
    }
}

SimpleMonotoneAssembly::~SimpleMonotoneAssembly(){

}

ParallelAssembly::ParallelAssembly(){

}

ParallelAssembly::ParallelAssembly(std::map<std::string, Object_Assembly *> &assemblies){


    // create OR nodes
    for(std::pair<std::string,Object_Assembly*> assembly : assemblies){
        this->_or_nodes.insert(std::pair<std::string,Or_node>(assembly.second->getAssemblyName(),Or_node(assembly.second)));
    }

    // create AND nodes

    this->createANDNode("a1","1_2_3_4_5",{"1_2_3_4","5"});
    this->createANDNode("a2","1_2_3_4_5",{"1_2_3_5","4"});
    this->createANDNode("a3","1_2_3_4",{"1_2_3","4"});
    this->createANDNode("a4","1_2_3",{"1_2","3"});
    this->createANDNode("a5","1_2",{"1","2"});
    this->createANDNode("a6","1_2_3_5",{"1_2_3","5"});
    this->createANDNode("a7","1_2_3_4",{"1_2_4","3"});
    this->createANDNode("a8","1_2_4",{"1_2","4"});
    this->createANDNode("a9","1_2_3_5",{"1_2","3_5"});
    this->createANDNode("a10","3_5",{"3","5"});
    this->createANDNode("a11","1_2_3_5",{"1_2_3","5"});
    this->createANDNode("a12","1_2_3_4_5",{"1_2_4","3_5"});

    // set root
    this->_root=&this->_or_nodes["1_2_3_4_5"];

    this->setLeafs({"1","2","3","4","5"});


}

ParallelAssembly::~ParallelAssembly(){

}

ComplexAssembly::ComplexAssembly(){

}

void ComplexAssembly::setupGraph(std::map<std::string, Object_Assembly *> &assemblies, unsigned n){


    // create OR nodes
    for(std::pair<std::string,Object_Assembly*> assembly : assemblies){
        this->_or_nodes.insert(std::pair<std::string,Or_node>(assembly.second->getAssemblyName(),Or_node(assembly.second)));
    }

    // find root
    for(std::pair<std::string,Object_Assembly*> assembly : assemblies){
        if(assembly.second->getPartNames()->size()==n){
            this->_root=&this->_or_nodes[assembly.second->getAssemblyName()];
            break;
        }
    }

    // find leafs
    for(std::pair<std::string,Object_Assembly*> assembly : assemblies){
        if(assembly.second->getPartNames()->size()==1){
            this->_leafs.insert(&this->_or_nodes[assembly.first]);
        }
    }

    unsigned action_id=0;
    for(unsigned i=n;i>1;i--){

        for(std::pair<std::string,Or_node> or_node : this->_or_nodes){
            std::set<std::string> assigned;
            if(or_node.second.getAssembly()->getPartNames()->size()==i){

                for(unsigned j=1;j<i;j++){


                    for(std::pair<std::string,Or_node> or_node_child : this->_or_nodes){
                        if(assigned.find(or_node_child.second.getID())!=assigned.end()){
                            continue;
                        }
                        if(or_node_child.second.getAssembly()->getPartNames()->size()==j){
                            if(!std::includes(or_node.second.getAssembly()->getPartNames()->begin(),or_node.second.getAssembly()->getPartNames()->end(),or_node_child.second.getAssembly()->getPartNames()->begin(),or_node_child.second.getAssembly()->getPartNames()->end())){
                                continue;
                            }
                            std::string comp_child = this->getComplementaryAssembly(or_node.first,or_node_child.first);
                            this->createANDNode("a"+boost::lexical_cast<std::string>(action_id),or_node.second.getID(),{or_node_child.second.getID(),comp_child});
                            assigned.insert(or_node_child.second.getID());
                            assigned.insert(comp_child);
                            action_id++;
                        }
                    }
                }
            }
        }
    }
}

ComplexAssembly::~ComplexAssembly(){

}

double ComplexAssembly::get3(){
    return 3;
}

std::set<Object_Assembly> ComplexAssembly::getAssemblies(unsigned n){

    std::set<Object_Assembly> assemblies;
    std::vector<std::string> parts;
    for(unsigned i=0;i<n;i++){
        assemblies.insert(Object_Assembly({boost::lexical_cast<std::string>(i+1)}));
        parts.push_back(boost::lexical_cast<std::string>(i+1));
    }

    std::vector<std::vector<std::string> > combs;
    std::vector<std::string> current;

    for(unsigned i=1;i<=parts.size();i++){
        combs.clear();
        math_functions::nchoosek(0,i,combs,current,parts);

        for(unsigned j=0;j<combs.size();j++){
            std::set<std::string> tmp_parts;
            for(unsigned k=0;k<combs[j].size();k++){
                tmp_parts.insert(combs[j][k]);
            }
            assemblies.insert(Object_Assembly(tmp_parts));
        }
    }
    return assemblies;
}

std::map<std::pair<std::string,std::string>, double> ComplexAssembly::getCosts(std::set<Object_Worker> worker){

    std::map<std::pair<std::string,std::string>, double> costs;

    for(Object_Worker w : worker){
        for(std::pair<std::string,And_node> and_node : this->_and_nodes){
            double c = rand() % 100 + 1;
            std::string id=w.getId();
            costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>(w.getId(),and_node.second.getID()),c));
        }
    }
    return costs;
}



//BinaryAssembly::BinaryAssembly(){

//}

//BinaryAssembly::~BinaryAssembly(){

//}

//void BinaryAssembly::setupGraph(std::map<std::string, Assembly *> &assemblies,unsigned p){

//    unsigned n=pow(2,p);

//    // create OR nodes
//    for(std::pair<std::string,Assembly*> assembly : assemblies){
//        this->_or_nodes.insert(std::pair<std::string,Or_node>(assembly.second->getAssemblyName(),Or_node(assembly.second)));
//    }

//    // find root
//    for(std::pair<std::string,Assembly*> assembly : assemblies){
//        if(assembly.second->getPartNames()->size()==n){
//            this->_root=&this->_or_nodes[assembly.second->getAssemblyName()];
//            break;
//        }
//    }


////    this->createANDNode("a0","1_10_11_12_13_14_15_16_2_3_4_5_6_7_8_9",{"1_10_11_12_13_14_15_16","2_3_4_5_6_7_8_9"});

////    this->createANDNode("a1","1_10_11_12_13_14_15_16",{"1_10_11_12","13_14_15_16"});

////    this->createANDNode("a2","1_10_11_12",{"1_10","11_12"});
////    this->createANDNode("a3","1_10",{"1","10"});
////    this->createANDNode("a4","11_12",{"11","12"});

////    this->createANDNode("a5","13_14_15_16",{"13_14","15_16"});
////    this->createANDNode("a6","13_14",{"13","14"});
////    this->createANDNode("a7","15_16",{"15","16"});

////    this->createANDNode("a8","2_3_4_5_6_7_8_9",{"2_3_4_5","6_7_8_9"});

////    this->createANDNode("a9","2_3_4_5",{"2_3","4_5"});
////    this->createANDNode("a10","2_3",{"2","3"});
////    this->createANDNode("a11","4_5",{"4","5"});

////    this->createANDNode("a12","6_7_8_9",{"6_7","8_9"});
////    this->createANDNode("a13","6_7",{"6","7"});
////    this->createANDNode("a14","8_9",{"8","9"});




//    // find leafs
//    for(std::pair<std::string,Assembly*> assembly : assemblies){
//        if(assembly.second->getPartNames()->size()==1){
//            this->_leafs.insert(&this->_or_nodes[assembly.first]);
//        }
//    }

//    unsigned action_id=0;
//    for(unsigned i=n;i>1;i/=2){

//        for(std::pair<std::string,Or_node> or_node : this->_or_nodes){
//            std::set<std::string> assigned;
//            if(or_node.second.getAssembly()->getPartNames()->size()==i){


//                for(std::pair<std::string,Or_node> or_node_child : this->_or_nodes){
//                    if(assigned.find(or_node_child.second.getID())!=assigned.end()){
//                        continue;
//                    }
//                    if(or_node_child.second.getAssembly()->getPartNames()->size()==i/2){
//                        if(!std::includes(or_node.second.getAssembly()->getPartNames()->begin(),or_node.second.getAssembly()->getPartNames()->end(),or_node_child.second.getAssembly()->getPartNames()->begin(),or_node_child.second.getAssembly()->getPartNames()->end())){
//                            continue;
//                        }
//                        std::string comp_child = this->getComplementaryAssembly(or_node.first,or_node_child.first);
//                        this->createANDNode("a"+boost::lexical_cast<std::string>(action_id),or_node.second.getID(),{or_node_child.second.getID(),comp_child});
//                        assigned.insert(or_node_child.second.getID());
//                        assigned.insert(comp_child);
//                        action_id++;
//                    }
//                }
//            }
//        }
//    }
//}

//std::set<Object_Assembly> BinaryAssembly::getAssemblies(unsigned p){

//    unsigned n=pow(2,p);

//    std::set<Object_Assembly> assemblies;
//    std::vector<std::string> parts;
//    std::set<std::string> root;
//    for(unsigned i=0;i<n;i++){
//        assemblies.insert(Assembly({boost::lexical_cast<std::string>(i+1)}));
//        parts.push_back(boost::lexical_cast<std::string>(i+1));
//        root.insert(boost::lexical_cast<std::string>(i+1));
//    }

//    std::set<Assembly> current_parent;
//    std::set<Assembly> current_children;

//    current_parent.insert(Assembly(root));

//    for(unsigned i=0;i<p;i++){
//        for(Assembly a : current_parent){
//            std::set<std::string> name = getFirstHalfAssembly(a);
//            current_children.insert(Assembly(name));
//            name=getSecondHalfAssembly(a);
//            current_children.insert(Assembly(name));
//        }
//        for(Assembly a : current_parent){
//            assemblies.insert(a);
//        }
//        current_parent.clear();
//        for(Assembly a : current_children){
//            current_parent.insert(a);
//        }
//        current_children.clear();
//    }
//    return assemblies;
//}


//std::map<std::string,std::map<std::string,double> > BinaryAssembly::getCosts(std::set<Object_Worker> worker){

//    std::map<std::string,std::map<std::string,double> > costs;

//    for(Worker w : worker){
//        for(std::pair<std::string,And_node> and_node : this->_and_nodes){
//            double c = rand() % 100 + 1;
//            costs[w.getID()].insert(std::pair<std::string,double>(and_node.second.getID(),c));
//        }
//    }
//    return costs;
//}


//std::set<std::string> BinaryAssembly::getFirstHalfAssembly(Object_Assembly& a){

//    const std::set<std::string>* parts = a.getPartNames();
//    std::set<std::string> parts_half;

//    if(parts->size()%2!=0){
//        std::cout<<"assembly not divisible by 2"<<std::endl;
//        exit(0);
//    }
//    unsigned i=0;
//    for(std::string str : *parts){
//        if(i>parts->size()/2-1){
//            break;
//        }
//        parts_half.insert(str);
//        i++;
//    }

//    return parts_half;
//}

//std::set<std::string> BinaryAssembly::getSecondHalfAssembly(Object_Assembly &a){

//    const std::set<std::string>* parts = a.getPartNames();
//    std::set<std::string> parts_half;

//    if(parts->size()%2!=0){
//        std::cout<<"assembly not divisible by 2"<<std::endl;
//        exit(0);
//    }
//    unsigned i=0;
//    for(std::string str : *parts){
//        if(i>parts->size()/2-1){
//            parts_half.insert(str);
//        }
//        i++;
//    }

//    return parts_half;
//}

//PaperAssembly::PaperAssembly(){

//}

//PaperAssembly::~PaperAssembly(){

//}


//std::set<Object_Assembly> PaperAssembly::getAssemblies(){

//    std::set<Assembly> assemblies;

//    PhysicalData physData;
//    physData._pos_x=0.6;
//    physData._pos_y=0.0;
//    physData._pos_z=0.0;
//    physData._pos_phi=0.0;
//    physData._width=0.0;

//    AssemblyData assemData;
//    assemData._pos_x=0.0;
//    assemData._pos_y=0.0;
//    assemData._pos_z=0.0;
//    assemData._pos_phi=0.0;

//    // atomic parts
//    assemblies.insert(Assembly({"1"},physData,assemData));
//    assemblies.insert(Assembly({"2"},physData,assemData));
//    assemblies.insert(Assembly({"3"},physData,assemData));
//    assemblies.insert(Assembly({"4"},physData,assemData));
//    assemblies.insert(Assembly({"5"},physData,assemData));
//    assemblies.insert(Assembly({"6"},physData,assemData));
//    assemblies.insert(Assembly({"7"},physData,assemData));
//    assemblies.insert(Assembly({"8"},physData,assemData));
////    assemblies.insert(Assembly({"9"},physData,assemData));
////    assemblies.insert(Assembly({"10"},physData,assemData));

//    physData._pos_x=0.0;
//    physData._pos_y=0.0;
//    physData._pos_z=0.0;
//    physData._pos_phi=0.0;
//    physData._width=0.0;

//    assemData._pos_x=0.0;
//    assemData._pos_y=0.0;
//    assemData._pos_z=0.0;
//    assemData._pos_phi=0.0;

//    // subassemblies

//    assemblies.insert(Assembly({"1","2"},physData,assemData));
//    assemblies.insert(Assembly({"3","4"},physData,assemData));
//    assemblies.insert(Assembly({"5","6"},physData,assemData));
//    assemblies.insert(Assembly({"7","8"},physData,assemData));

//    assemblies.insert(Assembly({"1","2","3","4"},physData,assemData));
//    assemblies.insert(Assembly({"5","6","7","8"},physData,assemData));

//    assemblies.insert(Assembly({"3","4","5","6"},physData,assemData));

//    assemblies.insert(Assembly({"1","2","3","4","5","6"},physData,assemData));
//    assemblies.insert(Assembly({"3","4","5","6","7","8"},physData,assemData));

//    assemblies.insert(Assembly({"1","2","3","4","5","6","7","8"},physData,assemData));


////    assemblies.insert(Assembly({"1","2"},physData,assemData));
////    assemblies.insert(Assembly({"2","3"},physData,assemData));
////    assemblies.insert(Assembly({"1","2","3"},physData,assemData));

////    assemblies.insert(Assembly({"4","5"},physData,assemData));

////    assemblies.insert(Assembly({"6","7"},physData,assemData));
////    assemblies.insert(Assembly({"6","7","8"},physData,assemData));
////    assemblies.insert(Assembly({"6","7","8","9"},physData,assemData));
////    assemblies.insert(Assembly({"6","7","8","9","10"},physData,assemData));

////    assemblies.insert(Assembly({"1","2","3","4","5"},physData,assemData));

////    assemblies.insert(Assembly({"4","5","6","7","8","9","10"},physData,assemData));

////    assemblies.insert(Assembly({"1","2","3","4","5","6","7","8","9","10"},physData,assemData));

//    return assemblies;
//}

//void PaperAssembly::setupGraph(std::map<std::string, Assembly *> &assemblies){

//    for(std::pair<std::string,Assembly*> assembly : assemblies){
//        this->_or_nodes.insert(std::pair<std::string,Or_node>(assembly.second->getAssemblyName(),Or_node(assembly.second)));
//    }

//    // create AND nodes

////    this->createANDNode("a1","1_2",{"1","2"});
////    this->createANDNode("a2","2_3",{"2","3"});
////    this->createANDNode("a3","1_2_3",{"1","2_3"});
////    this->createANDNode("a4","1_2_3",{"1_2","3"});
////    this->createANDNode("a5","4_5",{"4","5"});
////    this->createANDNode("a6","6_7",{"6","7"});
////    this->createANDNode("a7","6_7_8",{"6_7","8"});
////    this->createANDNode("a8","6_7_8_9",{"6_7_8","9"});
////    this->createANDNode("a9","10_6_7_8_9",{"6_7_8_9","10"});
////    this->createANDNode("a10","1_2_3_4_5",{"1_2_3","4_5"});
////    this->createANDNode("a11","10_4_5_6_7_8_9",{"10_6_7_8_9","4_5"});
////    this->createANDNode("a12","1_10_2_3_4_5_6_7_8_9",{"10_4_5_6_7_8_9","1_2_3"});
////    this->createANDNode("a13","1_10_2_3_4_5_6_7_8_9",{"10_6_7_8_9","1_2_3_4_5"});

//    this->createANDNode("a1","1_2",{"1","2"});
//    this->createANDNode("a2","3_4",{"3","4"});
//    this->createANDNode("a3","5_6",{"5","6"});
//    this->createANDNode("a4","7_8",{"7","8"});

//    this->createANDNode("a5","1_2_3_4",{"1_2","3_4"});
//    this->createANDNode("a6","5_6_7_8",{"5_6","7_8"});

//    this->createANDNode("a7","3_4_5_6",{"3_4","5_6"});

//    this->createANDNode("a8","1_2_3_4_5_6",{"3_4_5_6","1_2"});
//    this->createANDNode("a9","3_4_5_6_7_8",{"3_4_5_6","7_8"});

//    this->createANDNode("a10","1_2_3_4_5_6_7_8",{"1_2_3_4","5_6_7_8"});
//    this->createANDNode("a11","1_2_3_4_5_6_7_8",{"1_2_3_4_5_6","7_8"});
//    this->createANDNode("a12","1_2_3_4_5_6_7_8",{"1_2","3_4_5_6_7_8"});



//    // set root
//    this->_root=&this->_or_nodes["1_2_3_4_5_6_7_8"];

//    this->setLeafs({"1","2","3","4","5","6","7","8"});

////    // set root
////    this->_root=&this->_or_nodes["1_10_2_3_4_5_6_7_8_9"];

////    this->setLeafs({"1","2","3","4","5","6","7","8","9","10"});

//}


//std::map<std::string,std::map<std::string,double> > PaperAssembly::getCosts(std::set<Worker> worker){

//    std::map<std::string,std::map<std::string,double> > costs;

//    for(Worker w : worker){
//        for(std::pair<std::string,And_node> and_node : this->_and_nodes){
//            double c = rand() % 100 + 1;
//            costs[w.getID()].insert(std::pair<std::string,double>(and_node.second.getID(),c));
//        }
//    }
//    return costs;
//}


//std::map<std::string,std::map<std::string,double> > PaperAssembly::getCostsMinTime(std::set<Worker> worker){

//    std::map<std::string,std::map<std::string,double> > costs;

//    // human
//    costs["human"].insert(std::pair<std::string,double>("a1",5));
//    costs["human"].insert(std::pair<std::string,double>("a2",5));
//    costs["human"].insert(std::pair<std::string,double>("a3",10));
//    costs["human"].insert(std::pair<std::string,double>("a4",10));

//    costs["human"].insert(std::pair<std::string,double>("a5",3));

//    costs["human"].insert(std::pair<std::string,double>("a6",10));
//    costs["human"].insert(std::pair<std::string,double>("a7",15));
//    costs["human"].insert(std::pair<std::string,double>("a8",20));
//    costs["human"].insert(std::pair<std::string,double>("a9",5));

//    costs["human"].insert(std::pair<std::string,double>("a10",5));
//    costs["human"].insert(std::pair<std::string,double>("a11",20));
//    costs["human"].insert(std::pair<std::string,double>("a12",15));
//    costs["human"].insert(std::pair<std::string,double>("a13",100));

//    // robot1
//    costs["robot1"].insert(std::pair<std::string,double>("a1",20));
//    costs["robot1"].insert(std::pair<std::string,double>("a2",10));
//    costs["robot1"].insert(std::pair<std::string,double>("a3",100));
//    costs["robot1"].insert(std::pair<std::string,double>("a4",10));

//    costs["robot1"].insert(std::pair<std::string,double>("a5",20));

//    costs["robot1"].insert(std::pair<std::string,double>("a6",100));
//    costs["robot1"].insert(std::pair<std::string,double>("a7",10));
//    costs["robot1"].insert(std::pair<std::string,double>("a8",10));
//    costs["robot1"].insert(std::pair<std::string,double>("a9",5));

//    costs["robot1"].insert(std::pair<std::string,double>("a10",2000));
//    costs["robot1"].insert(std::pair<std::string,double>("a11",2000));
//    costs["robot1"].insert(std::pair<std::string,double>("a12",2000));
//    costs["robot1"].insert(std::pair<std::string,double>("a13",30));

//    // robot2
//    costs["robot2"].insert(std::pair<std::string,double>("a1",20));
//    costs["robot2"].insert(std::pair<std::string,double>("a2",10));
//    costs["robot2"].insert(std::pair<std::string,double>("a3",10));
//    costs["robot2"].insert(std::pair<std::string,double>("a4",10));

//    costs["robot2"].insert(std::pair<std::string,double>("a5",10));

//    costs["robot2"].insert(std::pair<std::string,double>("a6",5));
//    costs["robot2"].insert(std::pair<std::string,double>("a7",5));
//    costs["robot2"].insert(std::pair<std::string,double>("a8",10));
//    costs["robot2"].insert(std::pair<std::string,double>("a9",5));

//    costs["robot2"].insert(std::pair<std::string,double>("a10",2000));
//    costs["robot2"].insert(std::pair<std::string,double>("a11",2000));
//    costs["robot2"].insert(std::pair<std::string,double>("a12",2000));
//    costs["robot2"].insert(std::pair<std::string,double>("a13",10));
//    return costs;
//}

//std::map<std::string,std::map<std::string,double> > PaperAssembly::getCostsMinHuman(std::set<Worker> worker){

//    std::map<std::string,std::map<std::string,double> > costs;

//    // human
//    costs["human"].insert(std::pair<std::string,double>("a1",50));
//    costs["human"].insert(std::pair<std::string,double>("a2",50));
//    costs["human"].insert(std::pair<std::string,double>("a3",100));
//    costs["human"].insert(std::pair<std::string,double>("a4",100));

//    costs["human"].insert(std::pair<std::string,double>("a5",30));

//    costs["human"].insert(std::pair<std::string,double>("a6",100));
//    costs["human"].insert(std::pair<std::string,double>("a7",100));
//    costs["human"].insert(std::pair<std::string,double>("a8",200));
//    costs["human"].insert(std::pair<std::string,double>("a9",50));

//    costs["human"].insert(std::pair<std::string,double>("a10",50));
//    costs["human"].insert(std::pair<std::string,double>("a11",50));
//    costs["human"].insert(std::pair<std::string,double>("a12",50));
//    costs["human"].insert(std::pair<std::string,double>("a13",100));

////    // robot1
////    costs["robot1"].insert(std::pair<std::string,double>("a1",20));
////    costs["robot1"].insert(std::pair<std::string,double>("a2",10));
////    costs["robot1"].insert(std::pair<std::string,double>("a3",10));
////    costs["robot1"].insert(std::pair<std::string,double>("a4",10));

////    costs["robot1"].insert(std::pair<std::string,double>("a5",20));

////    costs["robot1"].insert(std::pair<std::string,double>("a6",100));
////    costs["robot1"].insert(std::pair<std::string,double>("a7",5));
////    costs["robot1"].insert(std::pair<std::string,double>("a8",10));
////    costs["robot1"].insert(std::pair<std::string,double>("a9",5));

////    costs["robot1"].insert(std::pair<std::string,double>("a10",200));
////    costs["robot1"].insert(std::pair<std::string,double>("a11",2000));
////    costs["robot1"].insert(std::pair<std::string,double>("a12",100));
////    costs["robot1"].insert(std::pair<std::string,double>("a13",30));

////    // robot2
////    costs["robot2"].insert(std::pair<std::string,double>("a1",20));
////    costs["robot2"].insert(std::pair<std::string,double>("a2",10));
////    costs["robot2"].insert(std::pair<std::string,double>("a3",10));
////    costs["robot2"].insert(std::pair<std::string,double>("a4",10));

////    costs["robot2"].insert(std::pair<std::string,double>("a5",10));

////    costs["robot2"].insert(std::pair<std::string,double>("a6",5));
////    costs["robot2"].insert(std::pair<std::string,double>("a7",5));
////    costs["robot2"].insert(std::pair<std::string,double>("a8",10));
////    costs["robot2"].insert(std::pair<std::string,double>("a9",5));

////    costs["robot2"].insert(std::pair<std::string,double>("a10",200));
////    costs["robot2"].insert(std::pair<std::string,double>("a11",2000));
////    costs["robot2"].insert(std::pair<std::string,double>("a12",100));
////    costs["robot2"].insert(std::pair<std::string,double>("a13",10));


//    costs["robot1"].insert(std::pair<std::string,double>("a1",20));
//    costs["robot1"].insert(std::pair<std::string,double>("a2",10));
//    costs["robot1"].insert(std::pair<std::string,double>("a3",10));
//    costs["robot1"].insert(std::pair<std::string,double>("a4",10));

//    costs["robot1"].insert(std::pair<std::string,double>("a5",20));

//    costs["robot1"].insert(std::pair<std::string,double>("a6",100));
//    costs["robot1"].insert(std::pair<std::string,double>("a7",10));
//    costs["robot1"].insert(std::pair<std::string,double>("a8",10));
//    costs["robot1"].insert(std::pair<std::string,double>("a9",5));

//    costs["robot1"].insert(std::pair<std::string,double>("a10",2000));
//    costs["robot1"].insert(std::pair<std::string,double>("a11",2000));
//    costs["robot1"].insert(std::pair<std::string,double>("a12",2000));
//    costs["robot1"].insert(std::pair<std::string,double>("a13",30));

//    // robot2
//    costs["robot2"].insert(std::pair<std::string,double>("a1",20));
//    costs["robot2"].insert(std::pair<std::string,double>("a2",10));
//    costs["robot2"].insert(std::pair<std::string,double>("a3",10));
//    costs["robot2"].insert(std::pair<std::string,double>("a4",10));

//    costs["robot2"].insert(std::pair<std::string,double>("a5",10));

//    costs["robot2"].insert(std::pair<std::string,double>("a6",5));
//    costs["robot2"].insert(std::pair<std::string,double>("a7",5));
//    costs["robot2"].insert(std::pair<std::string,double>("a8",10));
//    costs["robot2"].insert(std::pair<std::string,double>("a9",5));

//    costs["robot2"].insert(std::pair<std::string,double>("a10",2000));
//    costs["robot2"].insert(std::pair<std::string,double>("a11",2000));
//    costs["robot2"].insert(std::pair<std::string,double>("a12",2000));
//    costs["robot2"].insert(std::pair<std::string,double>("a13",10));
//    return costs;
//}

//TestAssembly::TestAssembly(){

//}

//TestAssembly::~TestAssembly(){

//}


//std::set<Assembly> TestAssembly::getAssemblies(){

//    std::set<Assembly> assemblies;

//    PhysicalData physData;
//    physData._pos_x=0.6;
//    physData._pos_y=0.0;
//    physData._pos_z=0.0;
//    physData._pos_phi=0.0;
//    physData._width=0.0;

//    AssemblyData assemData;
//    assemData._pos_x=0.0;
//    assemData._pos_y=0.0;
//    assemData._pos_z=0.0;
//    assemData._pos_phi=0.0;

//    // atomic parts
//    assemblies.insert(Assembly({"1"},physData,assemData));
//    assemblies.insert(Assembly({"2"},physData,assemData));
//    assemblies.insert(Assembly({"3"},physData,assemData));
//    assemblies.insert(Assembly({"4"},physData,assemData));

//    physData._pos_x=0.0;
//    physData._pos_y=0.0;
//    physData._pos_z=0.0;
//    physData._pos_phi=0.0;
//    physData._width=0.0;

//    assemData._pos_x=0.0;
//    assemData._pos_y=0.0;
//    assemData._pos_z=0.0;
//    assemData._pos_phi=0.0;

//    // subassemblies
//    assemblies.insert(Assembly({"1","2"},physData,assemData));
//    assemblies.insert(Assembly({"2","3"},physData,assemData));
//    assemblies.insert(Assembly({"1","2","3"},physData,assemData));
//    assemblies.insert(Assembly({"3","4"},physData,assemData));
//    assemblies.insert(Assembly({"1","2","3","4"},physData,assemData));


//    return assemblies;
//}

//void TestAssembly::setupGraph(std::map<std::string, Assembly *> &assemblies){

//    for(std::pair<std::string,Assembly*> assembly : assemblies){
//        this->_or_nodes.insert(std::pair<std::string,Or_node>(assembly.second->getAssemblyName(),Or_node(assembly.second)));
//    }

//    // create AND nodes

//    this->createANDNode("a1","1_2",{"1","2"});
////    this->createANDNode("a2","2_3",{"2","3"});
////    this->createANDNode("a3","1_2_3",{"1","2_3"});
////    this->createANDNode("a4","1_2_3",{"1_2","3"});
////    this->createANDNode("a5","1_2_3_4",{"1_2_3","4"});
//    this->createANDNode("a6","1_2_3_4",{"1_2","3_4"});
//    this->createANDNode("a7","3_4",{"3","4"});
//    // set root
//    this->_root=&this->_or_nodes["1_2_3_4"];

//    this->setLeafs({"1","2","3","4"});

//}


//std::map<std::string,std::map<std::string,double> > TestAssembly::getCosts(std::set<Worker> worker){

//    std::map<std::string,std::map<std::string,double> > costs;

//    for(Worker w : worker){
//        for(std::pair<std::string,And_node> and_node : this->_and_nodes){
//            double c = rand() % 100 + 1;
//            costs[w.getID()].insert(std::pair<std::string,double>(and_node.second.getID(),c));
//        }
//    }
//    return costs;
//}


//std::map<std::string,std::map<std::string,double> > TestAssembly::getCostsMinTime(std::set<Worker> worker){

//    std::map<std::string,std::map<std::string,double> > costs;

//    // human
//    costs["human"].insert(std::pair<std::string,double>("a1",20));
////    costs["human"].insert(std::pair<std::string,double>("a2",20));
////    costs["human"].insert(std::pair<std::string,double>("a3",20));
////    costs["human"].insert(std::pair<std::string,double>("a4",20));

////    costs["human"].insert(std::pair<std::string,double>("a5",3));
//    costs["human"].insert(std::pair<std::string,double>("a6",5));
//    costs["human"].insert(std::pair<std::string,double>("a7",15));

//    // robot1
//    costs["robot1"].insert(std::pair<std::string,double>("a1",5));
////    costs["robot1"].insert(std::pair<std::string,double>("a2",5));
////    costs["robot1"].insert(std::pair<std::string,double>("a3",10));
////    costs["robot1"].insert(std::pair<std::string,double>("a4",5));

////    costs["robot1"].insert(std::pair<std::string,double>("a5",20));
//    costs["robot1"].insert(std::pair<std::string,double>("a6",15));
//    costs["robot1"].insert(std::pair<std::string,double>("a7",20));


//    // robot2
//    costs["robot2"].insert(std::pair<std::string,double>("a1",100));
//    costs["robot2"].insert(std::pair<std::string,double>("a2",100));
//    costs["robot2"].insert(std::pair<std::string,double>("a3",100));
//    costs["robot2"].insert(std::pair<std::string,double>("a4",100));

//    costs["robot2"].insert(std::pair<std::string,double>("a5",20));
//    costs["robot2"].insert(std::pair<std::string,double>("a6",5));
//    costs["robot2"].insert(std::pair<std::string,double>("a7",10));

//    return costs;
//}
