#include "assembly_planner/thread_Agent.h"



Thread_Agent::Thread_Agent()
{
}

Thread_Agent::Thread_Agent(Object_Worker* w) {
    this->_agent = w;
    this->setStatus(idle);
}

Thread_Agent::~Thread_Agent()
{
}

void Thread_Agent::startTask(AssemblyAction a){
    this->setStatus(busy);
    boost::thread(boost::bind(&Thread_Agent::signal_startTask,this,a));
}

void Thread_Agent::finishedTask(std::string a){
    if(a=="success"){
        this->setStatus(finished);
    }
    if(a=="error"){
        this->setStatus(error);
    }
}

void Thread_Agent::confirmFinishedTask(){
    this->setStatus(idle);
}

void Thread_Agent::signal_startTask(AssemblyAction a){
    this->_sig_start(a);
}

Status Thread_Agent::getStatus(){
    Status tmp;
    this->_mtx.lock();
    tmp=this->_status;
    this->_mtx.unlock();
    return tmp;
}

void Thread_Agent::setStatus(Status s){
    this->_mtx.lock();
    this->_status=s;
    this->_mtx.unlock();
}

AgentWrapper::AgentWrapper(){
    
}

AgentWrapper::~AgentWrapper(){

}

AgentWrapper::AgentWrapper(std::string id){
    this->_id=id;
}

void AgentWrapper::startTask(AssemblyAction a){
    this->executeAction(a);
}

void AgentWrapper::signal_finished(std::string res){
    this->_sig_finished(res);
}

std::string AgentWrapper::getAssemblyPart(std::set<std::string> parts){
    for(std::string str : parts){
        std::set<std::string> part=splitString(str,"_");
        if(part.find("base")==part.end()){
            return str;
        }
    }
}

std::set<std::string> AgentWrapper::splitString(std::string str, std::string delimiter)
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
