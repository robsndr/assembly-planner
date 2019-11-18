#ifndef EDGEH_HPP
#define EDGEH_HPP


/*Forward declaration of Node so Conector knows about it
**/
#include <iostream>
#include "containers.hpp"

class Node;

/* Edge data class. Represens the interconnections within the graph.
**/
class Edge{
public:
    Edge( EdgeData );

    Edge( EdgeData, 
          Node *, 
          Node *
        );

    Node* getDestination() const;
    Node* getSource() const;

    void setDestination( Node* );
    void setSource( Node* );

    void print();

    std::size_t id_;
    EdgeData data_;
    
private:
    Node* to_;
    Node* from_;
};

/*
    Class representing the Nodes within the graph. 
**/ 
Edge::Edge( 
    EdgeData data
) {
    data_ = data;
}

/*
    Class representing the Nodes within the graph. 
**/ 
Edge::Edge(
    EdgeData data, 
    Node * source, 
    Node * sink
) {
    data_ = data;
    from_ = source;
    to_   = sink;
}

/*
    Class representing the Nodes within the graph. 
**/
inline Node*  
Edge::getDestination() const{
    return to_;
}

/*
    Class representing the Nodes within the graph. 
**/
inline Node* 
Edge::getSource() const{
    return from_;
}

inline void
Edge::setDestination(Node* node){
    to_ = node;
}

/*
    Class representing the Nodes within the graph. 
**/
inline void
Edge::setSource(Node* node){
    from_ = node;
}

#endif