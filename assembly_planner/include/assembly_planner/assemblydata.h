#ifndef ASSEMBLYDATA_H
#define ASSEMBLYDATA_H

#include <set>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <string>

struct PhysicalData{
    double _mass;

    double _pos_x;
    double _pos_y;
    double _pos_z;
    double _pos_phi;

    double _width;
    double _height;
};

struct AssemblyData{

    double _pos_x;
    double _pos_y;
    double _pos_z;
    double _pos_phi;

};

struct AssemblyAction {
	AssemblyAction() {
		this->_type = "idle";
	}
	AssemblyAction(std::string type, std::vector<std::string> agents, std::set<std::string> parts) {
		this->_type = type;
		this->_agents = agents;
		this->_parts = parts;
	}

	std::string _type;
	std::vector<std::string> _agents;
	std::set<std::string> _parts;

};


class Assembly{
public:
    Assembly(){}
    Assembly(std::set<std::string> parts){
        this->_parts=parts;
        this->_physData._pos_x=0;
        this->_physData._pos_y=0;
        this->_physData._pos_z=0;
    }
    Assembly(std::set<std::string> parts,PhysicalData physData,AssemblyData assemData){
        this->_parts=parts;
        this->_physData=physData;
        this->_assemblyData=assemData;
    }

    const PhysicalData* readPhysicalData() const{
        return &this->_physData;
    }

    PhysicalData* getPhysicalData(){
        return &this->_physData;
    }

    const AssemblyData* getAssemblyData() const{
        return &this->_assemblyData;
    }

    std::string getAssemblyName() const{
        std::string assembly;
        for(std::string part : this->_parts){
            assembly.append(part);
            assembly.append("_");
        }
        assembly.pop_back();
        return assembly;
    }

    const std::set<std::string>* getPartNames(){
        return &this->_parts;
    }

    bool operator < (const Assembly& a) const{
        return this->getAssemblyName()<a.getAssemblyName();
    }

private:
    PhysicalData _physData;
    AssemblyData _assemblyData;
    std::set<std::string> _parts;
};

struct WorkerData{
    std::string _id;

    // make workspace a class for itself with different kind of workspaces that can be derived
    double _workspace_x1;
    double _workspace_x2;
    double _workspace_y1;
    double _workspace_y2;

};

class Worker{
public:

    Worker(){}
    Worker(WorkerData data){
        this->_data=data;
    }

    std::string getID(){
        return this->_data._id;
    }

    bool isReachable(Assembly* assembly){
        if(assembly->readPhysicalData()->_pos_x<this->_data._workspace_x1 || assembly->readPhysicalData()->_pos_x>this->_data._workspace_x2){
            return false;
        }
        if(assembly->readPhysicalData()->_pos_y<this->_data._workspace_y1 || assembly->readPhysicalData()->_pos_y>this->_data._workspace_y2){
            return false;
        }
        return true;
    }

    const WorkerData* getWorkerData() const{
        return &this->_data;
    }

    bool operator < (const Worker& w) const{
        return this->_data._id<w.getWorkerData()->_id;
    }

private:
    WorkerData _data;
};


class And_node;

class Or_node{
public:
    Or_node();
    Or_node(Assembly* assembly,bool isIntermediate=false);

    Assembly* getAssembly();
    std::string getID();
    const std::set<And_node*> getChildren() const;
    void setChildren(std::set<And_node*> children);
    void addChild(And_node* child);
	bool isTmp();

private:

    std::string _id;
	bool _tmp;

    Assembly* _assembly;

    And_node* _parent;
    std::set<And_node*> _children;
};

class And_node{
public:
    And_node();
    And_node(std::string id);
    void setChildren(std::set<Or_node*> children);
    void setParent(Or_node* parent);
    Or_node* getParent();
    std::string getID();
    const std::set<Or_node *> getChildren() const;

private:

    Or_node* _parent;
    std::set<Or_node*> _children;
    std::string _id;
};

class ANDORgraph
{
public:
    ANDORgraph(){}
    ~ANDORgraph(){}

    std::string getComplementaryAssembly(std::string parent, std::string child){
        std::set<std::string> comp_child;
        const std::set<std::string>* parts_parent=this->_or_nodes[parent].getAssembly()->getPartNames();
        const std::set<std::string>* parts_child=this->_or_nodes[child].getAssembly()->getPartNames();
        for(std::string part : *parts_parent){
            if(parts_child->find(part)==parts_child->end()){
                comp_child.insert(part);
            }
        }
        return this->getAssemblyName(comp_child);
    }

    std::set<std::string> getComplementaryParts(std::string parent, std::string child){
        std::set<std::string> comp_child;
        const std::set<std::string>* parts_parent=this->_or_nodes[parent].getAssembly()->getPartNames();
        const std::set<std::string>* parts_child=this->_or_nodes[child].getAssembly()->getPartNames();
        for(std::string part : *parts_parent){
            if(parts_child->find(part)==parts_child->end()){
                comp_child.insert(part);
            }
        }
        return comp_child;
    }

    bool createANDNode(std::string id,std::string parent,std::set<std::string> children){
        this->_and_nodes.insert(std::pair<std::string,And_node>(id,And_node(id)));
        if(this->_or_nodes.find(parent)==this->_or_nodes.end()){
            std::cout<<"Warning: OR node with id "<<parent<<"does not exist"<<std::endl;
            return false;
        }
        this->_and_nodes[id].setParent(&this->_or_nodes[parent]);
        std::set<Or_node*> ptr_children;
        for(std::string str : children){
            ptr_children.insert(&this->_or_nodes[str]);
            if(this->_or_nodes.find(str)==this->_or_nodes.end()){
                std::cout<<"Warning: OR node with id "<<str<<"does not exist"<<std::endl;
                return false;
            }
        }

        this->_and_nodes[id].setChildren(ptr_children);

        this->_or_nodes[parent].addChild(&this->_and_nodes[id]);
        return true;
    }

    bool setLeafs(std::set<std::string> leafs){
        for(std::string str : leafs){
            if(this->_or_nodes.find(str)==this->_or_nodes.end()){
                std::cout<<"Warning: OR node with id "<<str<<"does not exist"<<std::endl;
                return false;
            }
            this->_leafs.insert(&this->_or_nodes[str]);
        }
        return true;
    }

    std::string getAssemblyName(std::set<std::string> parts) const{
        std::string assembly;
        for(std::string part : parts){
            assembly.append(part);
            assembly.append("_");
        }
        assembly.pop_back();
        return assembly;
    }

    Or_node* _root;

    std::map<std::string,And_node> _and_nodes;
    std::map<std::string,Or_node> _or_nodes;
    std::map<std::string,And_node> _h_nodes;

    std::set<Or_node*> _leafs;

    std::map<std::string,std::set<std::string> > _order_actions;
};

struct DataStorage{

    double getMinCosts(){
        double max=0;
        for(std::pair<std::string,std::map<std::string,double> > worker : this->_costMap_worker_to_action){
            for(std::pair<std::string,double> action : worker.second){
                if(action.second>max){
                    max=action.second;
                }
            }
        }
        double min=max;
        for(std::pair<std::string,std::map<std::string,double> > worker : this->_costMap_worker_to_action){
            for(std::pair<std::string,double> action : worker.second){
                if(action.second<min){
                    min=action.second;
                }
            }
        }
        return min;
    }

    std::map<std::string,Worker> _worker;
    std::map<std::string,Assembly> _assemblies_all;
    std::map<std::string,Assembly*> _assemblies_current;

    std::map<std::string,std::map<std::string,double> > _costMap_worker_to_action;

    ANDORgraph* _graph;

};
#endif // ASSEMBLYDATA_H
