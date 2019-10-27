#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>


/*Forward declaration of Node so Conector knows about it
**/
template <class TypeNode, class TypeConnector> 
class Node;


/* Connector data class. Represens the interconnections within the graph.
**/
template <class TypeNode, class TypeConnector> 
class Connector{
public:
    Connector(  Node<TypeNode, TypeConnector> *, 
                Node<TypeNode, TypeConnector> *
                );
    
    Connector(  Node<TypeNode, TypeConnector> *, 
                std::vector<Node<TypeNode, TypeConnector>*> &
                );

    std::vector< Node<TypeNode, TypeConnector>* > & getSuccessors() const;
    std::vector< Node<TypeNode, TypeConnector>* > & getPredecessors() const;

private:
    TypeConnector data_;
    std::vector<Node<TypeNode, TypeConnector>*> to_;
    std::vector<Node<TypeNode, TypeConnector>*> from_;
};

template <class TypeNode, class TypeConnector>  
Connector<TypeNode, TypeConnector>::Connector( 
    Node<TypeNode, TypeConnector> * source, 
    Node<TypeNode, TypeConnector> * sink
)
{
    from_.push_back(source);
    to_.push_back(sink);
}

template <class TypeNode, class TypeConnector> 
Connector<TypeNode, TypeConnector>::Connector( 
    Node<TypeNode, TypeConnector> * source, 
    std::vector<Node<TypeNode, TypeConnector>*> & sink 
)
{
    from_.push_back(source);
    to_ = sink;
}

template <class TypeNode, class TypeConnector> 
inline std::vector< Node<TypeNode, TypeConnector>* >& 
Connector<TypeNode,TypeConnector>::getSuccessors() const{
    return to_;
}

template <class TypeNode, class TypeConnector> 
inline std::vector< Node<TypeNode, TypeConnector>* >& 
Connector<TypeNode,TypeConnector>::getPredecessors() const{
    return from_;
}


/*
    Class representing the Nodes within the graph. 
    It generalizes to AND/OR graphs so Connectors are used instead of edges.
    Connectors can be seen as k-edges. One connector can reach up to k destination nodes.
**/
template <class TypeNode, class TypeConnector> 
class Node{
public:
    Node(std::string identifier, TypeNode data);

    bool hasSuccessor() const;
    bool hasPredecessor() const;
    void addPredecessor(Connector<TypeNode,TypeConnector> *);
    void addSuccessor(Connector<TypeNode,TypeConnector> *);
    std::size_t numberOfSuccessors() const;
    std::size_t numberOfPredecessors() const;
    // Connector * removeSuccessor(TypeId identifier);

    std::vector< Connector<TypeNode, TypeConnector>* > & getSuccessors() const;
    std::vector< Connector<TypeNode, TypeConnector>* > & getPredecessors() const;

    bool visited = false;

private:
    std::vector< Connector<TypeNode, TypeConnector>* > parents_;
    std::vector< Connector<TypeNode, TypeConnector>* > children_; 
    std::string id_;
    TypeNode data_;
};

template <class TypeNode, class TypeConnector> 
Node<TypeNode, TypeConnector>::Node(std::string identifier, TypeNode data){
    id_ = identifier;
    data_ = data;
}

template <class TypeNode, class TypeConnector> 
inline bool 
Node<TypeNode, TypeConnector>::hasSuccessor (void) const{
    return children_.empty();
}

template <class TypeNode, class TypeConnector> 
inline bool 
Node<TypeNode, TypeConnector>::hasPredecessor (void) const{
    return parents_.empty();
}

template <class TypeNode, class TypeConnector> 
inline std::vector< Connector<TypeNode, TypeConnector>* >&  
Node<TypeNode, TypeConnector>::getSuccessors(void) const{
    return children_;
}

template <class TypeNode, class TypeConnector> 
inline std::vector< Connector<TypeNode, TypeConnector>* >&  
Node<TypeNode, TypeConnector>::getPredecessors(void) const{
    return parents_;
}

template <class TypeNode, class TypeConnector> 
inline void 
Node<TypeNode, TypeConnector>::addPredecessor(
    Connector<TypeNode, TypeConnector> * predecessor
) {
    parents_.push_back(predecessor);
}

template <class TypeNode, class TypeConnector> 
inline void 
Node<TypeNode, TypeConnector>::addSuccessor(
    Connector<TypeNode, TypeConnector> * successor
) {
    children_.push_back(successor);
}

template <class TypeNode, class TypeConnector> 
inline std::size_t 
Node<TypeNode, TypeConnector>::numberOfSuccessors( ) const {
    return children_.size();
}

template <class TypeNode, class TypeConnector> 
inline std::size_t
Node<TypeNode, TypeConnector>::numberOfPredecessors() const{
    return parents_.size();
}