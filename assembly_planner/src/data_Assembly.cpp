#include "assembly_planner/data_Assembly.h"


Or_node::Or_node() {

}

Or_node::Or_node(Object_Assembly* assembly, bool isLocal) {
	this->_assembly = assembly;
    this->_local = isLocal;
    if (!this->_local) {
		this->_id = this->_assembly->getAssemblyName();
	}
	else {
		this->_id = this->_assembly->getAssemblyName() + "_local";
	}
}

Object_Assembly* Or_node::getAssembly() {
	return this->_assembly;
}

std::string Or_node::getID() {
	return this->_id;
}

const std::set<And_node*> Or_node::getChildren() const {
	return this->_children;
}

void Or_node::setChildren(std::set<And_node*> children) {
	this->_children = children;
}

void Or_node::addChild(And_node *child) {
	this->_children.insert(child);
}

bool Or_node::isLocal() {
	return this->_local;
}

And_node::And_node() {

}

And_node::And_node(std::string id) {
	this->_id = id;
}

void And_node::setChildren(std::set<Or_node *> children) {
	this->_children = children;
}

void And_node::setParent(Or_node *parent) {
	this->_parent = parent;
}

std::string And_node::getID() {
	return this->_id;
}

const std::set<Or_node*> And_node::getChildren() const {
	return this->_children;
}

Or_node* And_node::getParent() {
	return this->_parent;
}


Data_Assembly::Data_Assembly()
{
}


Data_Assembly::~Data_Assembly()
{
}

std::string ANDORgraph::getComplementaryAssembly(std::string parent, std::string child) {
	std::set<std::string> comp_child;
	const std::set<std::string>* parts_parent = this->_or_nodes[parent].getAssembly()->getPartNames();
	const std::set<std::string>* parts_child = this->_or_nodes[child].getAssembly()->getPartNames();
	for (std::string part : *parts_parent) {
		if (parts_child->find(part) == parts_child->end()) {
			comp_child.insert(part);
		}
	}
	return this->getAssemblyName(comp_child);
}

std::set<std::string> ANDORgraph::getComplementaryParts(std::string parent, std::string child) {
	std::set<std::string> comp_child;
	const std::set<std::string>* parts_parent = this->_or_nodes[parent].getAssembly()->getPartNames();
	const std::set<std::string>* parts_child = this->_or_nodes[child].getAssembly()->getPartNames();
	for (std::string part : *parts_parent) {
		if (parts_child->find(part) == parts_child->end()) {
			comp_child.insert(part);
		}
	}
	return comp_child;
}

bool ANDORgraph::createANDNode(std::string id, std::set<std::string> children1,std::set<std::string> children2) {
	this->_and_nodes.insert(std::pair<std::string, And_node>(id, And_node(id)));


    std::set<std::string> allparts;
    std::set<std::string> children;
    for(std::string c : children1){
            allparts.insert(c);
    }
    std::string c1=this->getAssemblyName(children1);
    children.insert(c1);
    for(std::string c : children2){
            allparts.insert(c);
    }
    std::string c2=this->getAssemblyName(children2);
    children.insert(c2);

    std::string parent=this->getAssemblyName(allparts);


	if (this->_or_nodes.find(parent) == this->_or_nodes.end()) {
		std::cout << "Warning: OR node with id " << parent << "does not exist" << std::endl;
		return false;
	}
	this->_and_nodes[id].setParent(&this->_or_nodes[parent]);
	std::set<Or_node*> ptr_children;
	for (std::string str : children) {
		ptr_children.insert(&this->_or_nodes[str]);
		if (this->_or_nodes.find(str) == this->_or_nodes.end()) {
			std::cout << "Warning: OR node with id " << str << "does not exist" << std::endl;
			return false;
		}
	}

	this->_and_nodes[id].setChildren(ptr_children);

	this->_or_nodes[parent].addChild(&this->_and_nodes[id]);
	return true;
}

bool ANDORgraph::setLeafs(std::set<std::string> leafs) {
	for (std::string str : leafs) {
		if (this->_or_nodes.find(str) == this->_or_nodes.end()) {
			std::cout << "Warning: OR node with id " << str << "does not exist" << std::endl;
			return false;
		}
		this->_leafs.insert(&this->_or_nodes[str]);
	}
	return true;
}

std::string ANDORgraph::getAssemblyName(std::set<std::string> parts) const {
	std::string assembly;
	for (std::string part : parts) {
		assembly.append(part);
		assembly.append("_");
	}
	assembly.pop_back();
	return assembly;
}

bool ANDORgraph::checkAssembly(std::map<std::string,Object_Worker> worker){

    std::cout<<"---------"<<std::endl;
    std::cout<<"---------"<<std::endl;
    std::cout<<"Output of assembly"<<std::endl;
    std::cout<<"Root node: "<<this->_root->getID()<<std::endl;
    std::cout<<"---------"<<std::endl;
    std::cout<<"OR nodes:"<<std::endl;
    std::cout<<"number of OR nodes: "<<this->_or_nodes.size()<<std::endl;
    for(std::pair<std::string,Or_node> or_node : this->_or_nodes){
        std::cout<<"id: "<<or_node.second.getID()<<", children: ";
        for(And_node* and_node : or_node.second.getChildren()){
            std::cout<<and_node->getID()<<",";
        }
        std::cout<<std::endl;
    }
    std::cout<<"---------"<<std::endl;
    std::cout<<"leafs:"<<std::endl;
    for(Or_node* leaf : this->_leafs){
        std::cout<<leaf->getID()<<",";
    }
    std::cout<<std::endl;
    std::cout<<"---------"<<std::endl;
    std::cout<<"AND nodes:"<<std::endl;
    std::cout<<"number of AND nodes: "<<this->_and_nodes.size()<<std::endl;
    for(std::pair<std::string,And_node> and_node : this->_and_nodes){
        std::cout<<"id: "<<and_node.second.getID()<<", children: ";
        for(Or_node* or_node : and_node.second.getChildren()){
            std::cout<<or_node->getID()<<",";
        }
        std::cout<<std::endl;
        std::cout<<"parent: "<<and_node.second.getParent()->getID()<<std::endl;
    }
    std::cout<<"---------"<<std::endl;
    std::cout<<"Action relations"<<std::endl;
    for(std::pair<std::string,std::set<std::string> > action : this->_order_actions){
        std::cout<<"action: "<<action.first<<std::endl;
        std::cout<<"conditional actions: ";
        for(std::string preAction : action.second){
            std::cout<<preAction<<",";
        }
        std::cout<<std::endl;
    }

    for(Or_node* part : this->_leafs){
        bool isReachable=false;

        for(std::pair<std::string,Object_Worker> w : worker){
            if(w.second.isReachable(part->getAssembly())){
                isReachable=true;
                break;
            }
        }
        if(!isReachable){
            std::cout<<"Assembly error! Not all parts are reachable by the workers!"<<std::endl;
            std::cout<<"Part: "<<part->getID()<<std::endl;
            std::cout<<"pos: "<<part->getAssembly()->_T<<std::endl;
            return false;
        }
    }
    return true;

}

std::set<std::string> ANDORgraph::splitString(std::string str, std::string delimiter)
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

Or_node* ANDORgraph::getOrNode(std::set<std::string> parts){
    return &this->_or_nodes[this->getAssemblyName(parts)];
}
