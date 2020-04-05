#pragma once

#include <unordered_set>

#include "types.hpp"

template <typename N>
struct Node
{
    Node(NodeIndex, N);

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

    NodeIndex id;
    N data;

  private:
  
    std::unordered_set<EdgeIndex> parents_;
    std::unordered_set<EdgeIndex> children_;
};

template <typename N>
Node<N>::Node(NodeIndex idx, N dat)
{
    id = idx;
    data = dat;
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
