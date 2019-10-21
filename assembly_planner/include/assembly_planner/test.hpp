#pragma once

#include "assembly_planner/objects.h"
#include "assembly_planner/data_Assembly.h"


class Assembly_Test : public ANDORgraph{
public:
    Assembly_Test();
    ~Assembly_Test();

    std::set<Object_Assembly> getAssemblies();
    void setupGraph(std::map<std::string,Object_Assembly>* assemblies);

private:


};

void testAssemblyPlaner();

void setupCosts(Memory* mem);
