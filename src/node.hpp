#pragma once

#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_set>

#include "edge.hpp"

using NodeIndex = size_t;

template <typename N>
struct Node
{
    Node(std::size_t identifier, N data);

    bool hasSuccessor() const;
    bool hasPredecessor() const;
    std::size_t numberOfSuccessors() const;
    std::size_t numberOfPredecessors() const;

    void addPredecessorEdge(EdgeIndex);
    void addSuccessorEdge(EdgeIndex);
    void removePredecessorEdge(EdgeIndex);
    void removeSuccessorEdge(EdgeIndex);

    std::unordered_set<NodeIndex> &successorEdges();
    std::unordered_set<NodeIndex> &predecessorEdges();

    std::size_t id;
    N data_;

private:
    std::unordered_set<EdgeIndex> parents_;
    std::unordered_set<EdgeIndex> children_;
};

template <typename N>
Node<N>::Node(std::size_t identifier, N data)
{
    id = identifier;
    data_ = data;
}

template <typename N>
inline bool
Node<N>::hasSuccessor(void) const
{
    return !children_.empty();
}

template <typename N>
inline bool
Node<N>::hasPredecessor(void) const
{
    return !parents_.empty();
}

template <typename N>
inline void
Node<N>::addPredecessorEdge(EdgeIndex predecessor)
{
    parents_.insert(predecessor);
}

template <typename N>
inline void
Node<N>::addSuccessorEdge(EdgeIndex successor)
{
    children_.insert(successor);
}

template <typename N>
inline void
Node<N>::removePredecessorEdge(
    std::size_t predId)
{
    parents_.erase(predId);
}

template <typename N>
inline void
Node<N>::removeSuccessorEdge(
    std::size_t succId)
{
    children_.erase(succId);
}

template <typename N>
inline std::size_t
Node<N>::numberOfSuccessors() const
{
    return children_.size();
}

template <typename N>
inline std::size_t
Node<N>::numberOfPredecessors() const
{
    return parents_.size();
}

template <typename N>
inline std::unordered_set<EdgeIndex> &
Node<N>::successorEdges()
{
    return children_;
}

template <typename N>
inline std::unordered_set<EdgeIndex> &
Node<N>::predecessorEdges()
{
    return parents_;
}
