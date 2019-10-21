#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>

#include "assembly_planner/objects.h"

class Data_Assembly
{
public:
	Data_Assembly();
	~Data_Assembly();
};

struct AssemblyAction {
	AssemblyAction() {
		this->_type = "idle";
	}
    AssemblyAction(std::string type, std::vector<std::string> agents, std::set<std::string> parts,std::string id) {
		this->_type = type;
		this->_agents = agents;
		this->_parts = parts;
        this->_id=id;
	}

	std::string _type;
	std::vector<std::string> _agents;
	std::set<std::string> _parts;
    std::string _id;

};

class And_node;

class Or_node {
public:
	Or_node();
    Or_node(Object_Assembly* assembly, bool isLocal = false);

	Object_Assembly* getAssembly();
	std::string getID();
	const std::set<And_node*> getChildren() const;
	void setChildren(std::set<And_node*> children);
	void addChild(And_node* child);
	bool isLocal();

private:

	std::string _id;
	bool _local;

	Object_Assembly* _assembly;

	std::set<And_node*> _children;
};

class And_node {
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
	ANDORgraph() {}
	~ANDORgraph() {}

    virtual std::set<Object_Assembly> getAssemblies() = 0;
    virtual void setupGraph(std::map<std::string,Object_Assembly>* assemblies) = 0;

	std::string getComplementaryAssembly(std::string parent, std::string child);
	std::set<std::string> getComplementaryParts(std::string parent, std::string child);
    bool createANDNode(std::string id, std::set<std::string> children1, std::set<std::string> children2);
	bool setLeafs(std::set<std::string> leafs);
	std::string getAssemblyName(std::set<std::string> parts) const;
    Or_node* getOrNode(std::set<std::string> parts);

    bool checkAssembly(std::map<std::string, Object_Worker> worker);

    std::set<std::string> splitString(std::string str, std::string delimiter);

	Or_node* _root;

	std::map<std::string, And_node> _and_nodes;
	std::map<std::string, Or_node> _or_nodes;
	std::map<std::string, And_node> _h_nodes;

	std::set<Or_node*> _leafs;

	std::map<std::string, std::set<std::string> > _order_actions;
	std::map<std::string, std::string> _types_actions;
};


class Object_ANDORgraph : public Object_Abstract{
public:
    Object_ANDORgraph(ANDORgraph* graph){
        this->_graph=graph;
    }
    ~Object_ANDORgraph(){}

    ANDORgraph* _graph;
};
