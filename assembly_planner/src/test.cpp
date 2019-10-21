#include "assembly_planner/test.hpp"


void setupCosts(Memory *mem){

    mem->insertMemory("map_cost",new Object_CostMap());

    std::map<std::pair<std::string, std::string>, double> costs;

    // human
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a1"),5));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a2"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a3"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a4"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a5"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a6"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a7"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a8"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a9"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a10"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a11"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a12"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a13"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a14"),50));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a15"),50));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a16"),50));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a17"),50));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a18"),50));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a19"),50));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a20"),50));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a21"),50));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a22"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("human","a23"),500));

    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a1"),1000));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a2"),5));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a3"),5));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a4"),5));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a5"),5));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a6"),100));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a7"),100));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a8"),50));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a9"),50));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a10"),5));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a11"),5));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a12"),20));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a13"),20));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a14"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a15"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a16"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a17"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a18"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a19"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a20"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a21"),500));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a22"),50));
    costs.insert(std::pair<std::pair<std::string,std::string>,double>(std::pair<std::string,std::string>("lwr3","a23"),50));


    static_cast<Object_CostMap*>(mem->getMemory("map_cost"))->_costmap=costs;
}

Assembly_Test::Assembly_Test(){

}

Assembly_Test::~Assembly_Test(){

}

std::set<Object_Assembly> Assembly_Test::getAssemblies(){
    std::set<Object_Assembly> assemblies;

    bnu::matrix<double> T=bnu::identity_matrix<double>(4);
    bnu::matrix<double> T_assembly=bnu::identity_matrix<double>(4);
    eulerRPY2Mat(T,0,0,0,0.5,0,0);



    // single parts
    assemblies.insert(Object_Assembly({"base"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"joint"},T,T_assembly));
    assemblies.insert(Object_Assembly({"shell1"},T,T_assembly));
    assemblies.insert(Object_Assembly({"shell2"},T,T_assembly));
    assemblies.insert(Object_Assembly({"screws1"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"screws2"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"screwdriver1"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"screwdriver2"},T_assembly,T_assembly));

    // compound parts
    assemblies.insert(Object_Assembly({"base","joint"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell1"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell1","screws1"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell2"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell2","screws2"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell1","shell2"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell1","shell2","screws1"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell1","shell2","screws2"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell1","shell2","screws1","screws2"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell1","screws1","screwdriver1"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell2","screws2","screwdriver2"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell1","shell2","screws1","screwdriver1"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell1","shell2","screws2","screwdriver2"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell1","shell2","screws1","screws2","screwdriver1"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell1","shell2","screws1","screws2","screwdriver2"},T_assembly,T_assembly));
    assemblies.insert(Object_Assembly({"base","joint","shell1","shell2","screws1","screws2","screwdriver1","screwdriver2"},T_assembly,T_assembly));

    return assemblies;
}

void Assembly_Test::setupGraph(std::map<std::string, Object_Assembly> *assemblies){

    std::map<std::string,Object_Assembly*> assemblies2;
    for(std::map<std::string,Object_Assembly>::iterator it=assemblies->begin();it!=assemblies->end();++it){
        assemblies2.insert(std::pair<std::string,Object_Assembly*>((*it).first,&(*it).second));
    }

    for(std::pair<std::string,Object_Assembly*> assembly : assemblies2){
        std::cout<<"OR_NODE: "<<assembly.second->getAssemblyName()<<std::endl;
        this->_or_nodes.insert(std::pair<std::string,Or_node>(assembly.second->getAssemblyName(),Or_node(assembly.second)));
    }

    this->createANDNode("a1",{"base"},{"joint"});
    this->createANDNode("a2",{"base","joint"},{"shell1"});
    this->createANDNode("a3",{"base","joint"},{"shell2"});
    this->createANDNode("a4",{"base","joint","shell2"},{"shell1"});
    this->createANDNode("a5",{"base","joint","shell1"},{"shell2"});
    this->createANDNode("a6",{"base","joint","shell1","screws1"},{"screwdriver1"});
    this->createANDNode("a7",{"base","joint","shell2","screws2"},{"screwdriver2"});
    this->createANDNode("a8",{"base","joint","shell1","shell2","screws1"},{"screwdriver1"});
    this->createANDNode("a9",{"base","joint","shell1","shell2","screws2"},{"screwdriver2"});
    this->createANDNode("a10",{"base","joint","shell1","screws1","screwdriver1"},{"shell2"});
    this->createANDNode("a11",{"base","joint","shell2","screws2","screwdriver2"},{"shell1"});
    this->createANDNode("a12",{"base","joint","shell1","shell2","screws1","screws2","screwdriver1"},{"screwdriver2"});
    this->createANDNode("a13",{"base","joint","shell1","shell2","screws1","screws2","screwdriver2"},{"screwdriver1"});
    this->createANDNode("a14",{"base","joint","shell2"},{"screws2"});
    this->createANDNode("a15",{"base","joint","shell1"},{"screws1"});
    this->createANDNode("a16",{"base","joint","shell1","shell2"},{"screws1"});
    this->createANDNode("a17",{"base","joint","shell1","shell2"},{"screws2"});
    this->createANDNode("a18",{"base","joint","shell1","shell2","screws2"},{"screws1"});
    this->createANDNode("a19",{"base","joint","shell1","shell2","screws1"},{"screws2"});
    this->createANDNode("a20",{"base","joint","shell1","shell2","screws2","screwdriver2"},{"screws2"});
    this->createANDNode("a21",{"base","joint","shell1","shell2","screws1","screwdriver1"},{"screws1"});
    this->createANDNode("a22",{"base","joint","shell1","shell2","screws1","screws2"},{"screwdriver1"});
    this->createANDNode("a23",{"base","joint","shell1","shell2","screws1","screws2"},{"screwdriver2"});

    this->_root=this->getOrNode({"base","joint","shell1","shell2","screws1","screws2","screwdriver1","screwdriver2"});

    this->setLeafs({"base","joint","shell1","shell2","screws1","screws2","screwdriver1","screwdriver2"});
}
