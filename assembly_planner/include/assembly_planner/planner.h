#pragma once

#include <map>
#include <list>
#include <vector>
#include <string>

#include "assembly_planner/a_star.h"
#include "assembly_planner/andorgraph_node.h"
#include "assembly_planner/data_Assembly.h"



class Planner
{
public:
    Planner();
    ~Planner();

    void linkToMemory(Memory* memory);
    algorithms::search::AStar* getAstar();
    std::map<std::string, std::list<AssemblyAction> > calculateNominalSequence(std::set<std::string> currentSubassemblies);

    std::map<std::string,std::set<std::string> > _skills;
private:
    Memory* _memory;
    algorithms::search::AStar _astar;

};

