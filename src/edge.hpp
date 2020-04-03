#pragma once

#include <iostream>
#include "containers.hpp"
#include "types.hpp"

template <typename E>
class Edge
{
  public:
    Edge(EdgeIndex, E);

    NodeIndex getDestination() const;
    NodeIndex getSource() const;

    void setDestination(NodeIndex);
    void setSource(NodeIndex);

    E data_;
    EdgeIndex id;

  private:
    NodeIndex to_;
    NodeIndex from_;
};

template <typename E>
Edge<E>::Edge(EdgeIndex idx, E data)
{
    id = idx;
    data_ = data;
}

template <typename E>
inline NodeIndex
Edge<E>::getDestination() const
{
    return to_;
}

template <typename E>
inline NodeIndex
Edge<E>::getSource() const
{
    return from_;
}

template <typename E>
inline void
Edge<E>::setDestination(NodeIndex node)
{
    to_ = node;
}

template <typename E>
inline void
Edge<E>::setSource(NodeIndex node)
{
    from_ = node;
}
