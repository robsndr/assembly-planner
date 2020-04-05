#pragma once

#include <unordered_map>

#include "node.hpp"
#include "edge.hpp"
#include "types.hpp"
#include "dotwriter.hpp"

template <typename N, typename E>
class Graph
{
  public:
    // Construction, Destruction
    Graph();
    Graph(const std::size_t, const std::size_t);
    Graph(const Graph<N,E>&) = default;
    ~Graph() = default;

    // General Information
    std::size_t numberOfNodes() const;
    std::size_t numberOfEdges() const;
    std::size_t numberOfSuccessors(const NodeIndex) const;
    std::size_t numberOfPredecessors(const NodeIndex) const;

    // Get address to object, mutable access
    Node<N>* getNode(const NodeIndex);
    Edge<E>* getEdge(const EdgeIndex);
    N& getNodeData(const NodeIndex);

    // Getters for neighbourhood relationships
    std::vector<Edge<E>*> getSuccessorEdges(const NodeIndex);
    std::vector<Edge<E>*> getPredecessorEdges(const NodeIndex);
    std::vector<Node<N>*> getSuccessorNodes(const NodeIndex);
    std::vector<Node<N>*> getPredecessorNodes(const NodeIndex);

    std::vector<NodeIndex> predecessorNodes(const NodeIndex);
    std::vector<NodeIndex> successorNodes(const NodeIndex);

    // Insertion
    NodeIndex insertNode(const N&);
    EdgeIndex insertEdge(const E&, const NodeIndex, const NodeIndex);

    // removal
    bool eraseNode(const NodeIndex);
    bool eraseEdge(const EdgeIndex);
    bool eraseEdge(const NodeIndex, const NodeIndex);

    // TODO: implement as friend. For template might be complications.
    // void print(DotWriter &);

    // Helpers
    std::pair<bool, EdgeIndex> findEdge(const NodeIndex, const NodeIndex) const;

    Node<N>* root;

  private:
    // Nodes
    std::unordered_map<NodeIndex, Node<N>> nodes_;
    NodeIndex free_node_id_;
    // Edges
    std::unordered_map<EdgeIndex, Edge<E>> edges_;
    EdgeIndex free_edge_id_;
};

template <typename N, typename E>
inline Graph<N, E>::Graph()
{
    free_node_id_ = 0;
    free_edge_id_ = 0;
}

template <typename N, typename E>
inline Graph<N, E>::Graph(
    const std::size_t number_of_nodes,
    const std::size_t number_of_edges)
{
    nodes_.reserve(number_of_nodes);
    edges_.reserve(number_of_edges);
    free_node_id_ = 0;
    free_edge_id_ = 0;

}

template <typename N, typename E>
inline std::size_t
Graph<N, E>::numberOfNodes() const
{
    return nodes_.size();
}

template <typename N, typename E>
inline std::size_t
Graph<N, E>::numberOfEdges() const
{
    return edges_.size();
}

template <typename N, typename E>
inline std::size_t
Graph<N, E>::numberOfSuccessors(
    const NodeIndex node) const
{
    return nodes_.at(node).numberOfSuccessors();
}

template <typename N, typename E>
inline std::size_t
Graph<N, E>::numberOfPredecessors(
    const NodeIndex node) const
{
    return nodes_.at(node).numberOfPredecessors();
}

template <typename N, typename E>
inline Node<N>*
Graph<N, E>::getNode(NodeIndex node_id)
{
    return &(nodes_.at(node_id));
}

template <typename N, typename E>
inline N&
Graph<N, E>::getNodeData(NodeIndex node_id)
{
    return nodes_.at(node_id).data;
}

template <typename N, typename E>
inline std::vector<Edge<E>*>
Graph<N, E>::getSuccessorEdges(
    const NodeIndex node)
{
    std::vector<Edge<E>*> edgeptrs;
    for(auto &e : nodes_.at(node).successorEdges())
    {
        edgeptrs.push_back(&edges_.at(e));
    }
    return edgeptrs;
}

template <typename N, typename E>
inline std::vector<Edge<E>*>
Graph<N, E>::getPredecessorEdges(
    const NodeIndex node)
{
    std::vector<Edge<E>*> edgeptrs;
    for(auto &e : nodes_.at(node).predecessorEdges())
    {
        edgeptrs.push_back(&edges_.at(e));
    }
    return edgeptrs;
}

template <typename N, typename E>
inline std::vector<Node<N>*>
Graph<N, E>::getSuccessorNodes(
    const NodeIndex node)
{
    std::vector<Node<N>*> nodeptrs;
    for(auto &e : nodes_.at(node).successorEdges())
    {
        auto n = edges_.at(e).getDestination();
        nodeptrs.push_back(&nodes_.at(n));
    }
    return nodeptrs;
}

template <typename N, typename E>
inline std::vector<Node<N>*>
Graph<N, E>::getPredecessorNodes(
    const NodeIndex node)
{
    std::vector<Node<N>*> nodeptrs;
    for(auto &e : nodes_.at(node).predecessorEdges())
    {
        auto n = edges_.at(e).getSource();
        nodeptrs.push_back(&nodes_.at(n));
    }
    return nodeptrs;
}

template <typename N, typename E>
inline std::vector<NodeIndex>
Graph<N, E>::successorNodes(
    const NodeIndex node)
{
    std::vector<NodeIndex> nodeptrs;
    for(auto &e : nodes_.at(node).successorEdges())
    {
        nodeptrs.push_back(edges_.at(e).getDestination());
    }
    return nodeptrs;
}

template <typename N, typename E>
inline std::vector<NodeIndex>
Graph<N, E>::predecessorNodes(
    const NodeIndex node)
{
    std::vector<NodeIndex> nodeptrs;
    for(auto &e : nodes_.at(node).predecessorEdges())
    {
        nodeptrs.push_back(edges_.at(e).getSource());
    }
    return nodeptrs;
}

template <typename N, typename E>
inline NodeIndex
Graph<N, E>::insertNode(const N& data)
{
    NodeIndex tid = free_node_id_;
    nodes_.insert(std::make_pair(free_node_id_, Node<N>(free_node_id_, data)));
    free_node_id_++;
    return tid;
}

template <typename N, typename E>
EdgeIndex
Graph<N, E>::insertEdge(
    const E& data,
    const NodeIndex src_node_id,
    const NodeIndex dest_node_id)
{
    auto e = Edge<E>(free_edge_id_, data);
    e.setDestination(dest_node_id);
    e.setSource(src_node_id);

    EdgeIndex eid = free_edge_id_;
    edges_.insert(std::make_pair(eid, std::move(e)));
    nodes_.at(src_node_id).addSuccessorEdge(eid);
    nodes_.at(dest_node_id).addPredecessorEdge(eid);
    free_edge_id_++;

    return eid;
}

template <typename N, typename E>
std::pair<bool, EdgeIndex>
Graph<N, E>::findEdge(
    const NodeIndex node_src,
    const NodeIndex node_dst) const
{
    bool success = false;
    EdgeIndex edge_index = -1;

    for (const auto& edge : edges_)
    {
        if (edge.second.getSource() == nodes_.at(node_src).id 
                && edge.second.getDestination() == nodes_.at(node_dst).id)
        {
            success = true;
            edge_index = edge.first;
            break;
        }
    }
    return std::make_pair(success, edge_index);
}

template <typename N, typename E>
bool Graph<N, E>::eraseEdge(
    const EdgeIndex edge_id)
{
    auto edge = edges_.at(edge_id);
    nodes_.at(edge.getSource()).removeSuccessorEdge(edge_id);
    nodes_.at(edge.getDestination()).removePredecessorEdge(edge_id);
    edges_.erase(edge_id);

    return true;
}

template <typename N, typename E>
bool Graph<N, E>::eraseEdge(
    const NodeIndex node_src,
    const NodeIndex node_dst)
{
    std::pair<bool, EdgeIndex> result = findEdge(node_src, node_dst);
    EdgeIndex eid = std::get<1>(result);

    if (!std::get<0>(result))
    {
        std::cerr << "Specified edge is not present in the graph." << std::endl;
        return false;
    }

    nodes_.at(node_dst).removeSuccessorEdge(eid);
    nodes_.at(node_dst).removePredecessorEdge(eid);
    edges_.erase(eid);

    return true;
}

// /* Print graph
// */
// template <typename N, typename E>
// void Graph<N, E>::print(DotWriter &writer)
// {
//     writer.write(nodes_);
// }
