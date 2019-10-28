#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
//TODO: Introduce Smart-Pointer instead.


/*Forward declaration of Node so Conector knows about it
**/
template <class TypeNode, class TypeEdge> 
class Node;


/* Edge data class. Represens the interconnections within the graph.
**/
template <class TypeNode, class TypeEdge> 
class Edge{
public:
    Edge( TypeEdge );

    Edge( TypeEdge, 
          Node<TypeNode, TypeEdge> *, 
          Node<TypeNode, TypeEdge> *
        );

    Node<TypeNode, TypeEdge>* getDestination() const;
    Node<TypeNode, TypeEdge>* getSource() const;

    void setDestination( Node<TypeNode, TypeEdge>* );
    void setSource( Node<TypeNode, TypeEdge>* );

private:
    std::string id_;
    TypeEdge data_;
    Node<TypeNode, TypeEdge>* to_;
    Node<TypeNode, TypeEdge>* from_;
};

template <class TypeNode, class TypeEdge>  
Edge<TypeNode, TypeEdge>::Edge( 
    TypeEdge data
) {
    data_ = data;
}

template <class TypeNode, class TypeEdge>  
Edge<TypeNode, TypeEdge>::Edge(
    TypeEdge data, 
    Node<TypeNode, TypeEdge> * source, 
    Node<TypeNode, TypeEdge> * sink
) {
    data_ = data;
    from_ = source;
    to_   = sink;
}

template <class TypeNode, class TypeEdge> 
inline Node<TypeNode, TypeEdge>*  
Edge<TypeNode,TypeEdge>::getDestination() const{
    return to_;
}

template <class TypeNode, class TypeEdge> 
inline Node<TypeNode, TypeEdge>* 
Edge<TypeNode,TypeEdge>::getSource() const{
    return from_;
}

template <class TypeNode, class TypeEdge> 
inline void
Edge<TypeNode,TypeEdge>::setDestination(Node<TypeNode, TypeEdge>* node){
    to_ = node;
}

template <class TypeNode, class TypeEdge> 
inline void
Edge<TypeNode,TypeEdge>::setSource(Node<TypeNode, TypeEdge>* node){
    from_ = node;
}

/*
    Class representing the Nodes within the graph. 
    It generalizes to AND/OR graphs so Connectors are used instead of edges.
    Connectors can be seen as k-edges. One connector can reach up to k destination nodes.
**/
template <class TypeNode, class TypeEdge> 
class Node{
public:
    Node(std::string identifier, TypeNode data);

    bool hasSuccessor() const;
    bool hasPredecessor() const;
    void addPredecessor(Edge<TypeNode,TypeEdge> *);
    void addSuccessor(Edge<TypeNode,TypeEdge> *);
    std::size_t numberOfSuccessors() const;
    std::size_t numberOfPredecessors() const;
    // Connector * removeSuccessor(TypeId identifier);

    std::vector< Edge<TypeNode, TypeEdge>* > & getSuccessors() const;
    std::vector< Edge<TypeNode, TypeEdge>* > & getPredecessors() const;

    bool visited = false;
    std::string id_;

private:
    std::vector< Edge<TypeNode, TypeEdge>* > parents_;
    std::vector< Edge<TypeNode, TypeEdge>* > children_; 
    TypeNode data_;
};

template <class TypeNode, class TypeEdge> 
Node<TypeNode, TypeEdge>::Node(std::string identifier, TypeNode data){
    id_ = identifier;
    data_ = data;
}

template <class TypeNode, class TypeEdge> 
inline bool 
Node<TypeNode, TypeEdge>::hasSuccessor (void) const{
    return children_.empty();
}

template <class TypeNode, class TypeEdge> 
inline bool 
Node<TypeNode, TypeEdge>::hasPredecessor (void) const{
    return parents_.empty();
}

template <class TypeNode, class TypeEdge> 
inline std::vector< Edge<TypeNode, TypeEdge>* >&  
Node<TypeNode, TypeEdge>::getSuccessors(void) const{
    return children_;
}

template <class TypeNode, class TypeEdge> 
inline std::vector< Edge<TypeNode, TypeEdge>* >&  
Node<TypeNode, TypeEdge>::getPredecessors(void) const{
    return parents_;
}

template <class TypeNode, class TypeEdge> 
inline void 
Node<TypeNode, TypeEdge>::addPredecessor(
    Edge<TypeNode, TypeEdge> * predecessor
) {
    parents_.push_back(predecessor);
}

template <class TypeNode, class TypeEdge> 
inline void 
Node<TypeNode, TypeEdge>::addSuccessor(
    Edge<TypeNode, TypeEdge> * successor
) {
    children_.push_back(successor);
}

template <class TypeNode, class TypeEdge> 
inline std::size_t 
Node<TypeNode, TypeEdge>::numberOfSuccessors( ) const {
    return children_.size();
}

template <class TypeNode, class TypeEdge> 
inline std::size_t
Node<TypeNode, TypeEdge>::numberOfPredecessors() const{
    return parents_.size();
}