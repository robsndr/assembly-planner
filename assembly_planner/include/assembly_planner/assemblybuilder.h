#ifndef ASSEMBLYBUILDER_H
#define ASSEMBLYBUILDER_H

#include <set>
#include <iostream>
#include <vector>
#include <utility>

#include "assembly_planner/data_Assembly.h"
#include "assembly_planner/assemblies.h"
#include "assembly_planner/andorgraph_node.h"
#include "assembly_planner/planer.h"

class AssemblyBuilder
{
public:
    AssemblyBuilder();
    AssemblyBuilder(std::set<Object_Assembly> assemblies, std::set<Object_Worker> worker, std::map<std::string, std::map<std::string, double> > costs, Memory* mem, Planer *plan);
    ~AssemblyBuilder();

    std::map<std::string, std::list<AssemblyAction> > calculateNominalSequence(std::map<std::string, Object_Assembly*> currentSubassemblies, std::map<std::string,AssemblyAction> appliedActions_in);
    std::map<std::string,Object_Assembly*> getLeafAssemblies();

    void applyAction(std::string action);
	bool isApplicable(std::string action);
	std::string joinAssemblies(std::set<std::string> parts);

    void connectToMemory(Memory* mem);

    void setANDORgraph(ANDORgraph* graph);
    void setCurrentConfiguration(std::map<std::string,Object_Assembly*> currentConf);
    ANDORgraph *getANDORgraph();
//    DataStorage *getDataStorage();
    std::map<std::string, Object_Assembly*> getAssemblies();
    std::map<std::string,Object_Assembly*> getCurrentConfiguration();

private:

    bool checkAssembly(ANDORgraph *assembly);

    Memory* _memory;

//    algorithms::search::AStar _astar;

//    DataStorage _data;

    Planer* _planner;
    std::map<std::string,Object_Assembly*> _currentParts;

};

#endif // ASSEMBLYBUILDER_H
