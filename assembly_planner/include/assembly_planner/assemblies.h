#ifndef ASSEMBLIES
#define ASSEMBLIES

#include <list>

#include <boost/lexical_cast.hpp>

#include "assembly_planner/data_Assembly.h"
#include "assembly_planner/math_functions.h"

class SimpleMonotoneAssembly : public ANDORgraph{
public:
    SimpleMonotoneAssembly();
    SimpleMonotoneAssembly(std::list<Object_Assembly*> parts, std::map<std::string, Object_Assembly>* assemblies);
    ~SimpleMonotoneAssembly();
};


class ParallelAssembly : public ANDORgraph{
public:
    ParallelAssembly();
    ParallelAssembly(std::map<std::string, Object_Assembly *> &assemblies);
    ~ParallelAssembly();

};

class ComplexAssembly : public ANDORgraph{
public:
    ComplexAssembly();
    ~ComplexAssembly();

    void setupGraph(std::map<std::string, Object_Assembly *> &assemblies,unsigned n);
    std::map<std::pair<std::string, std::string>, double> getCosts(std::set<Object_Worker> worker);

    double get3();
    std::set<Object_Assembly> getAssemblies(unsigned n);

};

//class BinaryAssembly : public ANDORgraph{
//public:
//    BinaryAssembly();
//    ~BinaryAssembly();

//    void setupGraph(std::map<std::string, Object_Assembly *> &assemblies, unsigned p);
//    std::map<std::string,std::map<std::string,double> > getCosts(std::set<Object_Worker> worker);


//    std::set<Object_Assembly> getAssemblies(unsigned p);

//    std::set<std::string> getFirstHalfAssembly(Object_Assembly& a);
//    std::set<std::string> getSecondHalfAssembly(Object_Assembly& a);

//};

//class PaperAssembly : public ANDORgraph{
//public:
//    PaperAssembly();
//    ~PaperAssembly();

//    void setupGraph(std::map<std::string, Assembly *> &assemblies);
//    std::map<std::string,std::map<std::string,double> > getCosts(std::set<Worker> worker);

//    std::map<std::string,std::map<std::string,double> > getCostsMinTime(std::set<Worker> worker);
//    std::map<std::string,std::map<std::string,double> > getCostsMinHuman(std::set<Worker> worker);



//    std::set<Assembly> getAssemblies();

//};

//class TestAssembly : public ANDORgraph{
//public:
//    TestAssembly();
//    ~TestAssembly();

//    void setupGraph(std::map<std::string, Assembly *> &assemblies);
//    std::map<std::string,std::map<std::string,double> > getCosts(std::set<Worker> worker);

//    std::map<std::string,std::map<std::string,double> > getCostsMinTime(std::set<Worker> worker);


//    std::set<Assembly> getAssemblies();

//};

#endif // ASSEMBLIES

