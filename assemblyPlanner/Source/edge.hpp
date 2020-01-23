#pragma once

#include <iostream>
#include "containers.hpp"

/* Forward declaration of Node so Edge knows about it
**/
class Node;

/* Edge data class. Represens the interconnections within the graph.
    Edges can be used as a generalised container for any datatype.
    Data contained within edges needs to be specified within the EdgeData class.
**/
class Edge
{
public:
    Edge(EdgeData);
    Edge(EdgeData, Node *, Node *);

    Node *getDestination() const;
    Node *getSource() const;

    void setDestination(Node *);
    void setSource(Node *);

    std::size_t id_;
    EdgeData data_;

private:
    Node *to_;
    Node *from_;
};

/*
    Class representing the Nodes within the graph. 
**/
Edge::Edge(
    EdgeData data)
{
    data_ = data;
}

/*
    Class representing the Nodes within the graph. 
**/
Edge::Edge(
    EdgeData data,
    Node *source,
    Node *sink)
{
    data_ = data;
    from_ = source;
    to_ = sink;
}

/*
    Class representing the Nodes within the graph. 
**/
inline Node *
Edge::getDestination() const
{
    return to_;
}

/*
    Class representing the Nodes within the graph. 
**/
inline Node *
Edge::getSource() const
{
    return from_;
}

inline void
Edge::setDestination(Node *node)
{
    to_ = node;
}

/*
    Class representing the Nodes within the graph. 
**/
inline void
Edge::setSource(Node *node)
{
    from_ = node;
}
