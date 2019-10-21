#include "assembly_planner/assembly_controller.h"
#include "assembly_planner/test.hpp"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "planner");

    Assembly_Controller* controller=new Assembly_Controller();

    Memory* memory=new Memory;


    std::set<Object_Worker> worker;


    std::set<std::string> s1;
    s1.insert("s1");

    Object_Worker human("human",-0.5,0.1,-0.3,0.6);
//    human.setSkills(s1);
    Object_Worker lwr3("lwr3",-0.1,0.7,-0.6,0.6);
    lwr3.setSkills(s1);
    human.setSkills(s1);

    worker.insert(human);
    worker.insert(lwr3);


    std::map<std::string,std::set<std::string> > skill_map;
    skill_map["a1"]=s1;
    skill_map["a2"]=s1;
    skill_map["a3"]=s1;
    skill_map["a4"]=s1;
    skill_map["a5"]=s1;
    skill_map["a6"]=s1;
    skill_map["a7"]=s1;
    skill_map["a8"]=s1;
    skill_map["a9"]=s1;
    skill_map["a10"]=s1;
    skill_map["a11"]=s1;
    skill_map["a12"]=s1;
    skill_map["a13"]=s1;
    skill_map["a14"]=s1;
    skill_map["a15"]=s1;
    skill_map["a16"]=s1;
    skill_map["a17"]=s1;
    skill_map["a18"]=s1;
    skill_map["a19"]=s1;
    skill_map["a20"]=s1;
    skill_map["a21"]=s1;
    skill_map["a22"]=s1;
    skill_map["a23"]=s1;

    ANDORgraph* graph = new Assembly_Test();

    controller->setupController(memory,graph,worker);
    controller->setSkills(skill_map);
    setupCosts(memory);

    controller->assemblyProcess();
    //std::map<std::string,AgentWrapper> agents;



    //agents.insert(std::pair<std::string,Agent_LWR3*>("lwr3",new Agent_LWR3("lwr3",this->_lwr,false)));
    //agents.insert(std::pair<std::string,Agent_Human*>("human",new Agent_Human("human")));

    return 0;
}
