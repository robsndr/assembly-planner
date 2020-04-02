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

using NodeIndex = size_t;

template <typename Visitor = VerboseGraphVisitor<std::size_t>>
class Graph
{
public:
    // Construction, Destruction
    Graph(const Visitor& = Visitor());
    Graph(const std::size_t, const std::size_t, const Visitor& = Visitor());
    Graph(const Graph<>&);
    ~Graph() = default;

    // General Information
    std::size_t numberOfNodes() const;
    std::size_t numberOfEdges() const;
    std::size_t numberOfEdgesFromNode(const std::size_t) const;
    std::size_t numberOfEdgesToNode(const std::size_t) const;

    Node* getNode(std::size_t);
    const Node& getNodeRef(NodeIndex);

    std::vector<Node*> getLeafNodes();

    // Access specific nodes/vertices.
    Edge* edgeFromNode(const std::size_t, const std::size_t);
    Edge* edgeToNode(const std::size_t, const std::size_t);
    std::vector<Node*> nodesFromNode(const std::size_t, const std::size_t);
    std::vector<Node*> nodesToNode(const std::size_t, const std::size_t);
    std::pair<bool, std::size_t> findEdge(const std::size_t, const std::size_t) const;

    // inserttion
    // nodes
    Node* insertNode(const NodeData&);
    Node* insertNode(const Node&);
    std::size_t insertNodes(const std::vector<Node*>&);
    // edges
    Edge& insertEdge(const EdgeData&, const std::size_t, const std::size_t);
    std::size_t insertEdges(const EdgeData&,
                            const std::size_t,
                            const std::vector<std::size_t>&);

    // copy source graph
    std::unordered_map<std::size_t, std::size_t> copy(const Graph<>&);

    // removal
    bool eraseNode(const std::size_t);
    bool eraseEdge(const std::size_t, std::size_t);
    std::pair<bool, std::size_t> findEdge(const std::size_t, const std::size_t);

    // TODO: implement as friend. For template might be complications.
    void print(DotWriter &);

    Node *root_;
    size_t root_node_id;

private:
    std::size_t findEdgeIndexHelper(Edge*);

    std::unordered_map<std::size_t, Node> nodes_;
    std::size_t free_node_id_;

    std::unordered_map<std::size_t, Edge> edges_;
    std::size_t free_edge_id_;
    
    Visitor visitor_;
};

/* Construct a graph
    @param visitor: Visitor to follow changes of integer indices of vertices and edges.
**/
template <typename Visitor>
inline Graph<Visitor>::Graph(
    const Visitor &visitor)
      : visitor_(visitor)
{
    free_node_id_ = 0;
    free_edge_id_ = 0;
}

/* Construct a graph with preallocating memory for nodes and edges.
    @numberOfVertices: Number of vertices.
    @numberOfEdges: Number of edges.
    @visitor: Visitor to follow changes of integer indices of vertices and edges.
**/
template <typename Visitor>
inline Graph<Visitor>::Graph(
    const std::size_t numberOfNodes,
    const std::size_t numberOfEdges,
    const Visitor& visitor)
      : visitor_(visitor)
{
    nodes_.reserve(numberOfNodes);
    edges_.reserve(numberOfEdges);
    free_node_id_ = 0;
    free_edge_id_ = 0;

}

/* Copy-Constructor. Clone a given graph.
    @graph: source graph which should be copied.
            The resulting graph is a clone of the original one.
**/
template <typename Visitor>
inline Graph<Visitor>::Graph(
    const Graph<> &graph)
{
    copy(graph);
}

/* Clone a given graph.
    @graph: source graph which should be copied.
            The resulting graph is a clone of the original one.
**/
template <typename Visitor>
std::unordered_map<std::size_t, std::size_t>
Graph<Visitor>::copy(
    const Graph<> &graph)
{
    nodes_.clear();
    edges_.clear();

    free_node_id_ = 0;
    free_edge_id_ = 0;

    visitor_ = graph.visitor_;
    std::unordered_map<std::size_t, std::size_t> index_map;

    for (const auto& node_tuple : graph.nodes_)
    {
        auto const new_node = insertNode(node_tuple.second.data_);
        index_map[node_tuple.second.id_] = new_node->id_;
    }

    root_ = &nodes_.at(index_map[graph.root_->id_]);

    for (const auto& node_tuple : graph.nodes_)
    {
        for (auto const& predecessor_edge : node_tuple.second.getPredecessors())
        {
            insertEdge(predecessor_edge->data_,
                       index_map[predecessor_edge->getSource()->id_],
                       index_map[node_tuple.second.id_]);
        }

        for (auto const& successor_edge : node_tuple.second.getSuccessors())
        {
            insertEdge(successor_edge->data_,
                       index_map[node_tuple.second.id_],
                       index_map[successor_edge->getDestination()->id_]);
        }
    }

    return index_map;
}

/* Get the number of nodes.
**/
template <typename Visitor>
inline std::size_t
Graph<Visitor>::numberOfNodes() const
{
    return nodes_.size();
}

/* Get the number of edges.
*/
template <typename Visitor>
inline std::size_t
Graph<Visitor>::numberOfEdges() const
{
    return edges_.size();
}
                     
/* Get the number of edges that originate from a given node.
    @node string-id of a node.
**/
template <typename Visitor>
inline std::size_t
Graph<Visitor>::numberOfEdgesFromNode(
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
template <typename Visitor>
inline std::size_t
Graph<Visitor>::numberOfEdgesToNode(
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
template <typename Visitor>
inline Node*
Graph<Visitor>::getNode(std::size_t node_id)
{
    return &(nodes_.at(node_id));
}

/* Get the number of edges that are incident to a given node.
    @node: id of a node.
**/
template <typename Visitor>
inline const Node& 
Graph<Visitor>::getNodeRef(NodeIndex node_id)
{
    return nodes_.at(node_id);
}

/* Get reference to all Nodes that don't have successors (leaves).
    \return: vector containting references to all leaf-nodes present in the graph.
**/
template <typename Visitor>
inline std::vector<Node*>
Graph<Visitor>::getLeafNodes()
{
    std::vector<Node*> temp;
    for (auto it = std::begin(nodes_); it != std::end(nodes_); it++)
    {
        if (!it->second.hasSuccessor())
        {
            temp.push_back(&(it->second));
        }
    }
    return temp;
}

/* Get the pointer to the j`th edge that originates from a given node.
    @node: id of a node.
    @j; number of edge. Between 0 and numberOfedgesFromNode(node) - 1.
**/
template <typename Visitor>
inline Edge*
Graph<Visitor>::edgeFromNode(
    const std::size_t node,
    const std::size_t j)
{
    if (nodes_.find(node) == nodes_.end())
    {
        std::cerr << std::endl
                  << "Error: edgeFromNode. Node " 
                  << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }
    return nodes_.at(node).children_[j];
}

/* Get the pointer to the j`th edge that is incident to a given node.
    @node: id of a node.
    @j: index of edge. Between 0 and numberOfedgesToNode(node) - 1.
**/
template <typename Visitor>
inline Edge*
Graph<Visitor>::edgeToNode(
    const std::size_t node,
    const std::size_t j)
{
    if (nodes_.find(node) == nodes_.end())
    {
        std::cerr << std::endl
                  << "Error: edgeToNode. Node " 
                  << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }
    return nodes_.at(node).parents_[j];
}

/* Get pointers to the nodes reachable from a given node via a specified edge.
    @node: id of a node.
    @j: index of the edge being used. Between 0 and numberOfedgesFromNode(node) - 1.
**/
template <typename Visitor>
inline std::vector<Node*>
Graph<Visitor>::nodesFromNode(
    const std::size_t node,
    const std::size_t j)
{
    if (nodes_.find(node) == nodes_.end())
    {
        std::cerr << std::endl
                  << "Error: nodesFromNode. Node " 
                  << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }
    return nodes_.at(node).getSuccessorNodes();
}

/* Get pointers to the nodes incident to a given node via a specified edge.
    @node: id of a node.
    @j: index of the edge being used. Between 0 and numberOfedgesFromNode(node) - 1.
**/
template <typename Visitor>
inline std::vector<Node*>
Graph<Visitor>::nodesToNode(
    const std::size_t node,
    const std::size_t j)
{
    if (nodes_.find(node) == nodes_.end())
    {
        std::cerr << std::endl
                  << "Error: nodesToNode. Node " 
                  << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }
    return nodes_.at(node).getPredecessorNodes();
}

/* Insert an additional Node.
    @node: string-id of the newly inserted node.
    @data: data asociated with the created node.
    \return: pointer to new node
**/
template <typename Visitor>
inline Node*
Graph<Visitor>::insertNode(const Node& node)
{
    size_t tid = free_node_id_;
    NodeData ndata = node.data_;
    nodes_.insert(std::make_pair(free_node_id_, Node(free_node_id_, ndata)));
    free_node_id_++;
    // visitor_.insertVertex(nodeId);
    return &(nodes_.at(tid));
}

/* Insert an additional Node.
    @node: string-id of the newly inserted node.
    @data: data asociated with the created node.
    \return: pointer to new node
**/
template <typename Visitor>
inline Node*
Graph<Visitor>::insertNode(const NodeData& data)
{
    size_t tid = free_node_id_;
    nodes_.insert(std::make_pair(free_node_id_, Node(free_node_id_, data)));
    free_node_id_++;
    // visitor_.insertVertex(nodeId);
    return &(nodes_.at(tid));
}

/* Insert multiple additional Nodes.
    @nodes: vector containing nodes to be inserted into the graph.
    \return: number of nodes present in the graph.
**/
template <typename Visitor>
inline std::size_t
Graph<Visitor>::insertNodes(
    const std::vector<Node*> &nodes)
{
    for (const auto node : nodes)
    {
        nodes_.insert(std::make_pair(node->id_, *node));
        free_node_id_++;
    }
    // for(const auto nd: nodes){
    //     visitor_.insertNodes(nd->id_, nodes.size());
    // }
    return nodes_.size();
}

/* Insert additional edge.
    @srcNodeId: id of the source node of the edge.
    @destNodeId: id of the destinaion node of the edge.
    \return Integer index of the newly inserted edge.
**/
template <typename Visitor>
Edge&
Graph<Visitor>::insertEdge(
    const EdgeData& data,
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

    auto e = Edge(data);
    e.setDestination(&(nodes_.at(destNodeId)));
    e.setSource(&(nodes_.at(srcNodeId)));

    edges_.insert(std::make_pair(free_edge_id_, std::move(e)));
    nodes_.at(srcNodeId).addSuccessor(&edges_.at(free_edge_id_));
    nodes_.at(destNodeId).addPredecessor(&edges_.at(free_edge_id_));

    free_edge_id_++;

    return edges_.at(free_edge_id_-1);
}

/* Insert additional edges.
    @srcNodeId: string-ids of the source nodes of the edges.
    @destNodeId: string-ids of the destinaion nodes of the edges.
    \return: number of edges present in the graph.
**/
template <typename Visitor>
inline std::size_t
Graph<Visitor>::insertEdges(
    const EdgeData& data,
    const std::size_t srcNodeId,
    const std::vector<std::size_t> &destNodeIds)
{
    for (auto const& destNodeId : destNodeIds)
    {
        insertEdge(data, srcNodeId, destNodeId);
    }
    return edges_.size();
}

/* Search for an edge
    @nodeSrc: first vertex of the edge.
    @nodeDst: second vertex of the edge.
    \return if an edge from nodeSrc to nodeDst exists, 
      pair.first is true and pair.second is the index of such an edge. 
      if no edge from nodeSrc to nodeDst exists, pair.first is false 
      and pair.second is undefined.
*/
template <typename Visitor>
std::pair<bool, std::size_t>
Graph<Visitor>::findEdge(
    const std::size_t nodeSrc,
    const std::size_t nodeDst)
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
        if (edge.second.getSource() == &nodes_[nodeSrc] && edge.second.getDestination() == &nodes_[nodeDst])
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
template <typename Visitor>
bool Graph<Visitor>::eraseEdge(
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

    nodes_.at(nodeSrc).removeSuccessor(nodeDst);
    nodes_.at(nodeDst).removePredecessor(nodeSrc);

    edges_.erase(edgeIndex);

    return true;
}

/* Find the index corresponding to a given edge pointer.
    @edge: pointer to edge edge which index to find.
*/
// template <typename Visitor>
// inline std::size_t
// Graph<Visitor>::findEdgeIndexHelper(Edge *edge)
// {
//     std::size_t edge_index;
//     for (size_t j = 0; j < edges_.size(); j++)
//     {
//         if (edges_[j] == edge)
//         {
//             edge_index = j;
//             break;
//         }
//     }
//     return edge_index;
// }

// /* Erase a Node and all edges concerning this Node.
//     @nodeId Integer index of the vertex to be erased.
// **/
// template <typename Visitor>
// bool Graph<Visitor>::eraseNode(
//     const std::size_t nodeId)
// {
//     if (nodes_.find(nodeId) == nodes_.end())
//     {
//         std::cerr << "Unable to find node to remove. "
//                   << "Node: " << nodeId << " not in graph. Was not removed."
//                   << std::endl;
//         return false;
//     }

//     Node *node_to_remove = nodes_[nodeId];
//     nodes_.erase(nodeId);

//     std::vector<Edge *> predecessors = node_to_remove->getPredecessors();
//     std::vector<Edge *> successors = node_to_remove->getSuccessors();
//     std::size_t temp_edge_index;

//     for (size_t j = 0; j < predecessors.size(); j++)
//     {
//         Edge *edge_to_remove = predecessors[j];
//         Node *predecessor_node = edge_to_remove->getSource();
//         predecessor_node->removeSuccessor(nodeId);

//         temp_edge_index = findEdgeIndexHelper(edge_to_remove);
//         edges_.erase(edges_.begin() + temp_edge_index);
//         delete edge_to_remove;
//     }

//     for (size_t j = 0; j < successors.size(); j++)
//     {
//         Edge *edge_to_remove = successors[j];
//         Node *successor_node = edge_to_remove->getDestination();
//         successor_node->removePredecessor(nodeId);

//         temp_edge_index = findEdgeIndexHelper(edge_to_remove);
//         edges_.erase(edges_.begin() + temp_edge_index);
//         delete edge_to_remove;
//     }

//     delete node_to_remove;

//     return true;
// }

// /* DEBUG: print graph.
// */
template <typename Visitor>
void Graph<Visitor>::print(DotWriter &writer)
{
    writer.write(nodes_);
}
