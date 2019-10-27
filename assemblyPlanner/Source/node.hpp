#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>


/*Forward declaration of Node so Conector knows about it
**/
template <class TypeData> 
class Node;


/* Connector data class. Represens the interconnections within the graph.
**/
template <class TypeNode, class TypeConnector> 
class Connector{
public:
    Connector(  Node<TypeNode> *, 
                Node<TypeNode> *
                );
    
    Connector(  Node<TypeNode> *, 
                std::vector<Node<TypeNode>*> &
                );

    inline std::vector< Node<TypeNode>* > & getSuccessors() const;
    inline std::vector< Node<TypeNode>* > & getPredecessors() const;

private:
    TypeConnector data_;
    std::vector<Node<TypeNode>*> to_;
    std::vector<Node<TypeNode>*> from_;
};

template <class TypeNode, class TypeConnector> 
inline Connector<TypeNode, TypeConnector>::Connector( 
    Node<TypeNode> * source, 
    Node<TypeNode> * sink
)
{
    from_.push_back(source);
    to_.push_back(sink);
}

template <class TypeNode, class TypeConnector> 
inline Connector<TypeNode, TypeConnector>::Connector( 
    Node<TypeNode> * source, 
    std::vector<Node<TypeNode>*> & sink 
)
{
    from_.push_back(source);
    to_ = sink;
}

template <class TypeNode, class TypeConnector> 
inline std::vector< Node<TypeNode>* > & Connector<TypeNode,TypeConnector>::getSuccessors() const{
    return to_;
}

template <class TypeNode, class TypeConnector> 
inline std::vector< Node<TypeNode>* > & Connector<TypeNode,TypeConnector>::getPredecessors() const{
    return from_;
}


/*
    Class representing the Nodes within the graph. 
    It generalizes to AND/OR graphs so Connectors are used instead of edges.
    Connectors can be seen as k-edges. One connector can reach up to k destination nodes.
**/
template <class TypeData> 
class Node{
public:
    Node(std::string identifier, TypeData data);

    bool hasSuccessor() const;
    bool hasPredecessor() const;
    void addPredecessor(Connector<TypeData> *);
    void addSuccessor(Connector<TypeData> *);
    std::size_t numberOfSuccessors() const;
    std::size_t numberOfPredecessors() const;
    // Connector * removeSuccessor(TypeId identifier);

    std::vector< Connector<TypeData>* > & getSuccessors() const;
    std::vector< Connector<TypeData>* > & getPredecessors() const;

    bool visited = false;

private:
    std::vector< Connector<TypeData>* > parents_;
    std::vector< Connector<TypeData>* > children_; 
    std::string id_;
    TypeData data_;
};

template <class TypeData> 
Node<TypeData>::Node(std::string identifier, TypeData data){
    id_ = identifier;
    data_ = data;
}

template <class TypeData> 
bool Node<TypeData>::hasSuccessor (void) const{
    return children_.empty();
}

template <class TypeData> 
bool Node<TypeData>::hasPredecessor (void) const{
    return parents_.empty();
}

template <class TypeData> 
std::vector< Connector<TypeData>* > &  Node<TypeData>::getSuccessors(void) const{
    return children_;
}

template <class TypeData> 
std::vector< Connector<TypeData>* > &  Node<TypeData>::getPredecessors(void) const{
    return parents_;
}

template <class TypeData> 
void Node<TypeData>::addPredecessor(Connector<TypeData> * predecessor){
    parents_.push_back(predecessor);
}

template <class TypeData> 
void Node<TypeData>::addSuccessor(Connector<TypeData> * successor){
    children_.push_back(successor);
}

template <class TypeData> 
std::size_t Node<TypeData>::numberOfSuccessors() const{
    return children_.size();
}

template <class TypeData> 
std::size_t Node<TypeData>::numberOfPredecessors() const{
    return parents_.size();
}