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
    // Contructors
    Edge(EdgeData);
    Edge(EdgeData, Node *, Node *);

    // Getters
    Node *getDestination() const;
    Node *getSource() const;

    // Setters
    void setDestination(Node *);
    void setSource(Node *);

    // Data contained in the edge.
    EdgeData data_;

private:
    Node *to_;
    Node *from_;
};

/* Edge Constructor.
    @data: data represented by the Edge.
**/
Edge::Edge(
    EdgeData data)
{
    data_ = data;
}

/* Edge Constructor.
    @data: data represented by the Edge. 
    @source: the node where the edge originates from.
    @sink: the node where the edge is directed to. 
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

/* Obtain the pointer to the edge destination.
    \return: pointer to the Node where the edge is directed to.
**/
inline Node *
Edge::getDestination() const
{
    return to_;
}

/* Obtain the pointer edge origin.
    \return: pointer to the Node where the edge originates from.
**/
inline Node *
Edge::getSource() const
{
    return from_;
}

/* Set the pointer of the edge destination.
    @node: pointer to the destination Node.
**/
inline void
Edge::setDestination(Node *node)
{
    to_ = node;
}

/* Set the pointer of the edge source.
    @node: pointer to the source Node.
**/
inline void
Edge::setSource(Node *node)
{
    from_ = node;
}
