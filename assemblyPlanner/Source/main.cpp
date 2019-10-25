#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>


template <class TypeId, class TypeData> 
class Connector{
public:
    Connector(Node * source, Node * sink);
    Connector(Node * source, std::vector<);
    Connector(Node * from, Node * to, TypeData value);

    TypeData getData(void);
    std::vector<Node*> & getSuccessors(void);

private:
    TypeData data_;
};


template <class TypeId, class TypeData> 
class Node{
public:
    Node(void);
    Node(TypeId identifier);
    Node(TypeId identifier, TypeData data);

    bool hasSuccessor(void);
    bool hasPredecessor(void);
    int addPredecessor(Connector * predecessor);
    int addSuccessor(Connector * successor);
    Edge * removeSuccessor(TypeId identifier);


    std::vector< Edge* > getSuccessors(void);
    std::vector< Edge* > getPredecessors(void);

    bool visited = false;

private:
    std::vector<Connector*> parents_;
    std::vector<Connector*> children_; 
    TypeId id_;
    TypeData data_;
};

int main(void){

   return 0;
}