#pragma once

#include <vector>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <exception>
#include <memory>

#include "node.hpp"
#include "visitor.hpp"
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
    N& getNodeD(const NodeIndex);

    Edge<E>* getEdge(const EdgeIndex);;
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
    std::unordered_map<std::size_t, Node<N>> nodes_;
    std::size_t free_node_id_;
    // Edges
    std::unordered_map<std::size_t, Edge<E>> edges_;
    std::size_t free_edge_id_;
};

/* Construct a graph
    @param visitor: N to follow changes of integer indices of vertices and edges.
**/
template <typename N, typename E>
inline Graph<N, E>::Graph()
{
    free_node_id_ = 0;
    free_edge_id_ = 0;
}

/* Construct a graph with preallocating memory for nodes and edges.
    @numberOfVertices: Number of vertices.
    @numberOfEdges: Number of edges.
    @visitor: N to follow changes of integer indices of vertices and edges.
**/
template <typename N, typename E>
inline Graph<N, E>::Graph(
    const std::size_t numberOfNodes,
    const std::size_t numberOfEdges)
{
    nodes_.reserve(numberOfNodes);
    edges_.reserve(numberOfEdges);
    free_node_id_ = 0;
    free_edge_id_ = 0;

}

/* Get the number of nodes.
**/
template <typename N, typename E>
inline std::size_t
Graph<N, E>::numberOfNodes() const
{
    return nodes_.size();
}

/* Get the number of edges.
*/
template <typename N, typename E>
inline std::size_t
Graph<N, E>::numberOfEdges() const
{
    return edges_.size();
}
                     
/* Get the number of edges that originate from a given node.
    @node string-id of a node.
**/
template <typename N, typename E>
inline std::size_t
Graph<N, E>::numberOfSuccessors(
    const std::size_t node) const
{
    if (nodes_.find(node) == nodes_.end())
    {
        std::cerr << std::endl
                  << "Error: numberOfEdgesFromNode - node " 
                  << node << " not in graph." << std::endl;
        return 0;
    }
    return nodes_.at(node).numberOfSuccessors();
}

/* Get the number of edges that are incident to a given node.
    @node: string-id of a node.
**/
template <typename N, typename E>
inline std::size_t
Graph<N, E>::numberOfPredecessors(
    const std::size_t node) const
{
    if (nodes_.find(node) == nodes_.end())
    {
        std::cerr << std::endl
                  << "Error: numberOfEdgesFromNode. Node " 
                  << node << " not in graph." << std::endl;
        return 0;
    }
    return nodes_.at(node).numberOfPredecessors();
}

/* Get the number of edges that are incident to a given node.
    @node: id of a node.
**/
template <typename N, typename E>
inline Node<N>*
Graph<N, E>::getNode(std::size_t node_id)
{
    return &(nodes_.at(node_id));
}

template <typename N, typename E>
inline N&
Graph<N, E>::getNodeD(std::size_t node_id)
{
    return nodes_.at(node_id).data_;
}

/* Get the pointer to the j`th edge that originates from a given node.
    @node: id of a node.
    @j; number of edge. Between 0 and numberOfedgesFromNode(node) - 1.
**/
template <typename N, typename E>
inline std::vector<Edge<E>*>
Graph<N, E>::getSuccessorEdges(
    const std::size_t node)
{
    if (nodes_.find(node) == nodes_.end())
    {
        std::cerr << std::endl
                  << "Error: edgeFromNode. Node " 
                  << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }
    std::vector<Edge<E>*> edgeptrs;
    for(auto &e : nodes_.at(node).successorEdges())
    {
        edgeptrs.push_back(&edges_.at(e));
    }
    return edgeptrs;
}

// /* Get the pointer to the j`th edge that is incident to a given node.
//     @node: id of a node.
//     @j: index of edge. Between 0 and numberOfedgesToNode(node) - 1.
// **/
template <typename N, typename E>
inline std::vector<Edge<E>*>
Graph<N, E>::getPredecessorEdges(
    const std::size_t node)
{
    if (nodes_.find(node) == nodes_.end())
    {
        std::cerr << std::endl
                  << "Error: edgeToNode. Node " 
                  << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }
    std::vector<Edge<E>*> edgeptrs;
    for(auto &e : nodes_.at(node).predecessorEdges())
    {
        edgeptrs.push_back(&edges_.at(e));
    }
    return edgeptrs;
}

/* Get pointers to the nodes reachable from a given node via a specified edge.
    @node: id of a node.
    @j: index of the edge being used. Between 0 and numberOfedgesFromNode(node) - 1.
**/
template <typename N, typename E>
inline std::vector<Node<N>*>
Graph<N, E>::getSuccessorNodes(
    const std::size_t node)
{
    if (nodes_.find(node) == nodes_.end())
    {
        std::cerr << std::endl
                  << "Error: nodesFromNode. Node " 
                  << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }

    std::vector<Node<N>*> nodeptrs;
    for(auto &e : nodes_.at(node).successorEdges())
    {
        auto n = edges_.at(e).getDestination();
        nodeptrs.push_back(&nodes_.at(n));
    }
    return nodeptrs;
}

/* Get pointers to the nodes incident to a given node via a specified edge.
    @node: id of a node.
    @j: index of the edge being used. Between 0 and numberOfedgesFromNode(node) - 1.
**/
template <typename N, typename E>
inline std::vector<Node<N>*>
Graph<N, E>::getPredecessorNodes(
    const std::size_t node)
{
    if (nodes_.find(node) == nodes_.end())
    {
        std::cerr << std::endl
                  << "Error: nodesToNode. Node " 
                  << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }
    std::vector<Node<N>*> nodeptrs;
    for(auto &e : nodes_.at(node).predecessorEdges())
    {
        auto n = edges_.at(e).getSource();
        nodeptrs.push_back(&nodes_.at(n));
    }
    return nodeptrs;
}

/* Get pointers to the nodes reachable from a given node via a specified edge.
    @node: id of a node.
    @j: index of the edge being used. Between 0 and numberOfedgesFromNode(node) - 1.
**/
template <typename N, typename E>
inline std::vector<NodeIndex>
Graph<N, E>::successorNodes(
    const std::size_t node)
{
    if (nodes_.find(node) == nodes_.end())
    {
        std::cerr << std::endl
                  << "Error: nodesFromNode. Node " 
                  << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }
    std::vector<NodeIndex> nodeptrs;
    for(auto &e : nodes_.at(node).successorEdges())
    {
        nodeptrs.push_back(edges_.at(e).getDestination());
    }
    return nodeptrs;
}

/* Get pointers to the nodes incident to a given node via a specified edge.
    @node: id of a node.
    @j: index of the edge being used. Between 0 and numberOfedgesFromNode(node) - 1.
**/
template <typename N, typename E>
inline std::vector<NodeIndex>
Graph<N, E>::predecessorNodes(
    const std::size_t node)
{
    if (nodes_.find(node) == nodes_.end())
    {
        std::cerr << std::endl
                  << "Error: nodesToNode. Node " 
                  << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }

    std::vector<NodeIndex> nodeptrs;
    for(auto &e : nodes_.at(node).predecessorEdges())
    {
        nodeptrs.push_back(edges_.at(e).getSource());
    }
    return nodeptrs;
}

/* Insert an additional Node.
    @node: string-id of the newly inserted node.
    @data: data asociated with the created node.
    \return: pointer to new node
**/
template <typename N, typename E>
inline NodeIndex
Graph<N, E>::insertNode(const N& data)
{
    size_t tid = free_node_id_;
    nodes_.insert(std::make_pair(free_node_id_, Node<N>(free_node_id_, data)));
    free_node_id_++;
    // visitor_.insertVertex(nodeId);
    return tid;
}

/* Insert additional edge.
    @srcNodeId: id of the source node of the edge.
    @destNodeId: id of the destinaion node of the edge.
    \return Integer index of the newly inserted edge.
**/
template <typename N, typename E>
EdgeIndex
Graph<N, E>::insertEdge(
    const E& data,
    const std::size_t srcNodeId,
    const std::size_t destNodeId)
{
    if (nodes_.find(srcNodeId) == nodes_.end())
    {
        std::cerr << "Unable to create edge. "
                  << "Node " << srcNodeId << " not in graph." << std::endl;
        throw std::range_error("Unable to create edge.");
    }
    if (nodes_.find(destNodeId) == nodes_.end())
    {
        std::cerr << "Unable to create edge. "
                  << "Node" << destNodeId << " not in graph." << std::endl;
        throw std::range_error("Unable to create edge.");
    }

    auto e = Edge<E>(free_edge_id_, data);
    e.setDestination(destNodeId);
    e.setSource(srcNodeId);

    edges_.insert(std::make_pair(free_edge_id_, std::move(e)));
    nodes_.at(srcNodeId).addSuccessorEdge(free_edge_id_);
    nodes_.at(destNodeId).addPredecessorEdge(free_edge_id_);

    free_edge_id_++;

    return free_edge_id_-1;
}

/* Search for an edge
    @nodeSrc: first vertex of the edge.
    @nodeDst: second vertex of the edge.
    \return if an edge from nodeSrc to nodeDst exists, 
      pair.first is true and pair.second is the index of such an edge. 
      if no edge from nodeSrc to nodeDst exists, pair.first is false 
      and pair.second is undefined.
*/
template <typename N, typename E>
std::pair<bool, EdgeIndex>
Graph<N, E>::findEdge(
    const NodeIndex nodeSrc,
    const NodeIndex nodeDst) const
{

    // check if given nodes exist in the graph.
    if (nodes_.find(nodeSrc) == nodes_.end())
    {
        std::cerr << "Edge search: source node does not exist in graph. "
                  << "Node " << nodeSrc << " not in graph." << std::endl;
        return std::make_pair(false, 0);
    }

    if (nodes_.find(nodeDst) == nodes_.end())
    {
        std::cerr << "Edge search: destination node does not exist in graph. "
                  << "Node " << nodeDst << " not in graph." << std::endl;
        return std::make_pair(false, 0);
    }

    // search the edges for the given connection.
    bool success = false;
    std::size_t edge_index = -1;

    for (const auto& edge : edges_)
    {
        if (edge.second.getSource() == nodes_.at(nodeSrc).id && edge.second.getDestination() == nodes_.at(nodeDst).id)
        {
            success = true;
            edge_index = edge.first;
            break;
        }
    }

    return std::make_pair(success, edge_index);
}

// /* Erase an edge.
//     @edgeIndex: Integer index of the edge to be erased.
// */
template <typename N, typename E>
bool Graph<N, E>::eraseEdge(
    const EdgeIndex edgeIndex)
{
    if (edges_.find(edgeIndex) == edges_.end())
    {
        std::cerr << "eraseEdge: Edge does not exist" << std::endl;
        return false;
    }

    auto edge = edges_.at(edgeIndex);
    nodes_.at(edge.getSource()).removeSuccessorEdge(edgeIndex);
    nodes_.at(edge.getDestination()).removePredecessorEdge(edgeIndex);
    edges_.erase(edgeIndex);

    return true;
}

// /* Erase an edge.
//     @edgeIndex: Integer index of the edge to be erased.
// */
template <typename N, typename E>
bool Graph<N, E>::eraseEdge(
    const std::size_t nodeSrc,
    const std::size_t nodeDst)
{
    if (nodes_.find(nodeSrc) == nodes_.end())
    {
        std::cerr << "Edge search: source node does not exist in graph. "
                  << "Node " << nodeSrc << " not in graph." << std::endl;
        return false;
    }

    if (nodes_.find(nodeDst) == nodes_.end())
    {
        std::cerr << "Edge search: destination node does not exist in graph. "
                  << "Node " << nodeDst << " not in graph." << std::endl;
        return false;
    }

    std::pair<bool, std::size_t> result = findEdge(nodeSrc, nodeDst);
    std::size_t edgeIndex = std::get<1>(result);

    if (!std::get<0>(result))
    {
        std::cerr << "eraseEdge: Specified edge is not present in the graph." << std::endl;
        std::cerr << "eraseEdge: Edge was not removed." << std::endl;
        return false;
    }

    nodes_.at(nodeDst).removeSuccessorEdge(edgeIndex);
    nodes_.at(nodeDst).removePredecessorEdge(edgeIndex);
    edges_.erase(edgeIndex);

    return true;
}

// /* Print graph
// */
// template <typename N, typename E>
// void Graph<N, E>::print(DotWriter &writer)
// {
//     writer.write(nodes_);
// }
