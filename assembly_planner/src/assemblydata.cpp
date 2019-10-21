#include "assembly_planner/assemblydata.h"

Or_node::Or_node(){

}

Or_node::Or_node(Assembly* assembly,bool isIntermediate){
    this->_assembly=assembly;
	this->_tmp = isIntermediate;
    if(!isIntermediate){
        this->_id=this->_assembly->getAssemblyName();
    }
    else{
        this->_id=this->_assembly->getAssemblyName()+"int";
    }
}

Assembly* Or_node::getAssembly(){
    return this->_assembly;
}

std::string Or_node::getID(){
    return this->_id;
}

const std::set<And_node*> Or_node::getChildren() const{
    return this->_children;
}

void Or_node::setChildren(std::set<And_node*> children){
    this->_children=children;
}

void Or_node::addChild(And_node *child){
    this->_children.insert(child);
}

bool Or_node::isTmp() {
	return this->_tmp;
}

And_node::And_node(){

}

And_node::And_node(std::string id){
    this->_id=id;
}

void And_node::setChildren(std::set<Or_node *> children){
    this->_children=children;
}

void And_node::setParent(Or_node *parent){
    this->_parent=parent;
}

std::string And_node::getID(){
    return this->_id;
}

const std::set<Or_node*> And_node::getChildren() const{
    return this->_children;
}

Or_node* And_node::getParent(){
    return this->_parent;
}
