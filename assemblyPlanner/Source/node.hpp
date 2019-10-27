#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

//Forward declaration of Node so Conector knows about it
template <class TypeId, class TypeData> 
class Node;


/*
    Connector data class. Represens the interconnections within the graph.
*/
template <class TypeId, class TypeData> 
class Connector{
public:
    Connector(  Node<TypeId,TypeData> * source, 
                Node<TypeId,TypeData> * sink 
                );
    
    Connector(  Node<TypeId,TypeData> * source, 
                std::vector<Node<TypeId,TypeData>*> & sink 
                );

    TypeData data_;
    std::vector<Node<TypeId,TypeData>*> to_;
    std::vector<Node<TypeId,TypeData>*> from_;
};

template <class TypeId, class TypeData> 
inline Connector<TypeId,TypeData>::Connector( 
    Node<TypeId,TypeData> * source, 
    Node<TypeId,TypeData> * sink
)
{
    from_.push_back(source);
    to_.push_back(sink);
}

template <class TypeId, class TypeData> 
inline Connector<TypeId,TypeData>::Connector( 
    Node<TypeId,TypeData> * source, 
    std::vector<Node<TypeId,TypeData>*> & sink 
)
{
    from_.push_back(source);
    to_ = sink;
}

template <class TypeId, class TypeData> 
inline std::vector<Node<TypeId,TypeData>*> & getSuccessors(void){
}

/*
    Class representing the Nodes within the graph. 
    It generalizes to AND/OR graphs so Connectors are used instead of edges.
    Connectors can be seen as k-edges. One connector can reach up to k destination nodes.
*/
template <class TypeId, class TypeData> 
class Node{
public:
    Node(TypeId identifier, TypeData data);

    bool hasSuccessor(void) const;
    bool hasPredecessor(void) const;
    void addPredecessor(Connector<TypeId,TypeData> * predecessor);
    void addSuccessor(Connector<TypeId,TypeData> * successor);
    // Connector * removeSuccessor(TypeId identifier);


    std::vector< Connector<TypeId,TypeData>* > & getSuccessors(void) const;
    std::vector< Connector<TypeId,TypeData>* > & getPredecessors(void) const;

    bool visited = false;

private:
    std::vector< Connector<TypeId,TypeData>* > parents_;
    std::vector< Connector<TypeId,TypeData>* > children_; 
    TypeId id_;
    TypeData data_;
};

template <class TypeId, class TypeData> 
Node<TypeId,TypeData>::Node(TypeId identifier, TypeData data){
    id_ = identifier;
    data_ = data;
}

template <class TypeId, class TypeData> 
bool Node<TypeId,TypeData>::hasSuccessor (void) const{
    return children_.empty();
}

template <class TypeId, class TypeData> 
bool Node<TypeId,TypeData>::hasPredecessor (void) const{
    return parents_.empty();
}

template <class TypeId, class TypeData> 
std::vector< Connector<TypeId,TypeData>* > &  Node<TypeId,TypeData>::getSuccessors(void) const{
    return children_;
}

template <class TypeId, class TypeData> 
std::vector< Connector<TypeId,TypeData>* > &  Node<TypeId,TypeData>::getPredecessors(void) const{
    return parents_;
}

template <class TypeId, class TypeData> 
void Node<TypeId,TypeData>::addPredecessor(Connector<TypeId,TypeData> * predecessor){
    parents_.push_back(predecessor);
}

template <class TypeId, class TypeData> 
void Node<TypeId,TypeData>::addSuccessor(Connector<TypeId,TypeData> * successor){
    children_.push_back(successor);
}