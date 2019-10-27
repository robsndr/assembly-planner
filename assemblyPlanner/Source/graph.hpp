#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cassert>
#include <cstddef>
#include <iterator> // std::random_access_iterator
#include <vector>
#include <set> 
#include <iostream>
#include <utility> // std::pair
#include <algorithm> // std::fill

#include <string>
#include <unordered_map>
#include "node.hpp"
#include "visitor.hpp"

template<typename connectorData, typename nodeData, typename Visitor = IdleGraphVisitor<std::string> >
class Graph{
public:
    // Construction
    Graph(const Visitor& = Visitor());
    Graph(const std::size_t, const std::size_t, const Visitor& = Visitor());

    // General Information
    std::size_t numberOfNodes() const;
    std::size_t numberOfConnectors() const;
    std::size_t numberOfConnectorsFromNode(const std::string) const;
    std::size_t numberOfConnectorsToNode(const std::string) const;

    // Access specific nodes/vertices.
    Connector<nodeData, connectorData> * connectorFromNode(const std::string, const std::size_t) const;
    Connector<nodeData, connectorData> * connectorToNode(const std::string, const std::size_t) const;
    std::vector<Node<nodeData, connectorData>*>& nodesFromNode(const std::string, const std::size_t) const;
    std::vector<Node<nodeData, connectorData>*>& nodesToNode(const std::string, const std::size_t) const;
    // std::pair<bool, std::size_t> findEdge(const std::size_t, const std::size_t) const;

    // manipulation
    std::size_t insertNode(std::string, nodeData);
    std::size_t insertNodes(std::vector<Node<nodeData, connectorData>>& nodes);

    std::size_t insertEdge(const std::size_t, const std::size_t);
    void eraseVertex(const std::size_t);
    void eraseEdge(const std::size_t);

    // Review
private:
    void insertAdjacenciesForEdge(const std::size_t);
    void eraseAdjacenciesForEdge(const std::size_t);

    std::unordered_map<std::string, Node> nodes_;
    std::vector<Connector> connectors_;
    Visitor visitor_;
};


/* Construct a graph
    @param visitor: Visitor to follow changes of integer indices of vertices and edges.
**/
template<typename connectorData, typename nodeData, typename Visitor>
inline 
Graph<connectorData, nodeData, Visitor>::Graph(
    const Visitor& visitor
)
:   nodes_(),
    connectors_(),
    visitor_(visitor)
{}


/* Construct a graph with preallocating memory for given Edges.
    @numberOfVertices: Number of vertices.
    @numberOfEdges: Number of edges.
    @visitor: Visitor to follow changes of integer indices of vertices and edges.
**/
template<typename connectorData, typename nodeData, typename Visitor>
inline 
Graph<connectorData, nodeData, Visitor>::Graph(
    const std::size_t numberOfNodes,
    const std::size_t numberOfConnectors,
    const Visitor& visitor
)
:   nodes_(),
    connectors_(),
    visitor_(visitor)
{
    connectors_.reserve(numberOfConnectors);
    visitor_.insertVertices(0, numberOfVertices);
}
    
/* Get the number of nodes.
**/
template<typename connectorData, typename nodeData, typename Visitor>
inline std::size_t
Graph<connectorData, nodeData, Visitor>::numberOfNodes() const { 
    return nodes_.size(); 
}

/* Get the number of connectors.
*/
template<typename connectorData, typename nodeData, typename Visitor>
inline std::size_t
Graph<connectorData, nodeData, Visitor>::numberOfConnectors() const { 
    return connectors_.size(); 
}

/* Get the number of connectors that originate from a given node.
    @node string-id of a node.
**/
template<typename connectorData, typename nodeData, typename Visitor>
inline std::size_t
Graph<connectorData, nodeData, Visitor>::numberOfConnectorsFromNode(
    const std::string node
) const { 
    return nodes_[node].numberOfChildren();
}

/* Get the number of edges that are incident to a given vertex.
    @node: string-id of a node.
**/
template<typename connectorData, typename nodeData, typename Visitor>
inline std::size_t
Graph<connectorData, nodeData, Visitor>::numberOfConnectorsToNode(
    const std::string node
) const { 
    return nodes_[node].numberOfParents();
}

/* Get the pointer to the j`th connector that originates from a given node.
    @node: string-id of a node.
    @j; number of Connector. Between 0 and numberOfConnectorsFromNode(node) - 1.
**/
template<typename connectorData, typename nodeData, typename Visitor>
inline Connector<nodeData, connectorData>*
Graph<connectorData, nodeData, Visitor>::connectorFromNode(
    const std::string node,
    const std::size_t j
) const {
    return nodes_[node].children[j];
}

/* Get the pointer to the j`th connector that is incident to a given node.
    @node: string-id of a node.
    @j: index of Connector. Between 0 and numberOfConnectorsToNode(node) - 1.
**/
template<typename connectorData, typename nodeData, typename Visitor>
inline Connector<nodeData, connectorData>*
Graph<connectorData, nodeData, Visitor>::connectorToNode(
    const std::string node,
    const std::size_t j
) const {
    return nodes_[node].parents[j];
}

/* Get pointers to the nodes reachable from a given node via a specified connector.
    @node: string-id of a node.
    @j: index of the connector being used. Between 0 and numberOfConnectorsFromNode(node) - 1.
**/
template<typename connectorData, typename nodeData, typename Visitor>
inline std::vector<Node<nodeData, connectorData>*> &
Graph<connectorData, nodeData, Visitor>::nodesFromNode(
    const std::string node,
    const std::size_t j
) const {
    return nodes_[node].children[j].getSuccessors();
}

/* Get pointers to the nodes incident to a given node via a specified connector.
    @node: string-id of a node.
    @j: index of the connector being used. Between 0 and numberOfConnectorsFromNode(node) - 1.
**/
template<typename connectorData, typename nodeData, typename Visitor>
inline std::vector<Node<nodeData, connectorData>*> &
Graph<connectorData, nodeData, Visitor>::nodesToNode(
    const std::string node,
    const std::size_t j
) const {
    return nodes_[node].parents[j].getPredecessors();
}


/* Insert an additional Node.
    @node: string-id of the newly inserted node.
    @data: data asociated with the created node.
    \return: number of nodes present in the graph.
**/
template<typename connectorData, typename nodeData, typename Visitor>
inline std::size_t
Graph<connectorData, nodeData, Visitor>::insertNode(std::string node, nodeData data) {
    nodes_.push_back(Node<nodeData,connectorData>(node, data));
    visitor_.insertVertex(vertices_.size() - 1);
    return nodes_.size() - 1;
}

/* Insert multiple additional Nodes.
    @nodes: vector containing nodes to be inserted into the graph.
    \return: number of nodes present in the graph.
**/
template<typename connectorData, typename nodeData, typename Visitor>
inline std::size_t
Graph<connectorData, nodeData, Visitor>::insertNodes(
    std::vector<Node<nodeData, connectorData>>& nodes
) {
    nodes_.insert(nodes_.end(), nodes.begin(), nodes.end());
    std::size_t position = ndoes_.size();
    visitor_.insertVertices(position, number);
    return position;
}

/* Insert an additional edge.
    @vertexIndex0 Integer index of the first vertex in the edge.
    @vertexIndex1 Integer index of the second vertex in the edge.
    \return Integer index of the newly inserted edge.
**/ 
template<typename connectorData, typename nodeData, typename Visitor>
inline std::size_t
Graph<connectorData, nodeData, Visitor>::insertEdge(
    const std::size_t vertexIndex0,
    const std::size_t vertexIndex1
) {
    assert(vertexIndex0 < numberOfVertices()); 
    assert(vertexIndex1 < numberOfVertices()); 
    
    if(multipleEdgesEnabled()) {
insertEdgeMark:
        edges_.push_back(Edge(vertexIndex0, vertexIndex1));
        std::size_t edgeIndex = edges_.size() - 1;
        insertAdjacenciesForEdge(edgeIndex);
        visitor_.insertEdge(edgeIndex);  
        return edgeIndex;
    }
    else {
        std::pair<bool, std::size_t> p = findEdge(vertexIndex0, vertexIndex1);
        if(p.first) { // edge already exists
            return p.second;
        }
        else {
            goto insertEdgeMark;
        }
    }
}

/// Erase a vertex and all edges connecting this vertex.
///
/// \param vertexIndex Integer index of the vertex to be erased.
/// 
template<typename connectorData, typename nodeData, typename Visitor>
void 
Graph<connectorData, nodeData, Visitor>::eraseVertex(
    const std::size_t vertexIndex
) {
    assert(vertexIndex < numberOfVertices()); 

    // erase all edges connected to the vertex
    while(vertices_[vertexIndex].size() != 0) {
        eraseEdge(vertices_[vertexIndex].begin()->edge());
    }

    if(vertexIndex == numberOfVertices()-1) { // if the last vertex is to be erased        
        vertices_.pop_back(); // erase vertex
        visitor_.eraseVertex(vertexIndex);
    }
    else { // if a vertex is to be erased which is not the last vertex
        // move last vertex to the free position:

        // collect indices of edges affected by the move
        std::size_t movingVertexIndex = numberOfVertices() - 1;
        std::set<std::size_t> affectedEdgeIndices;
        for(Vertex::const_iterator it = vertices_[movingVertexIndex].begin();
        it != vertices_[movingVertexIndex].end(); ++it) {
            affectedEdgeIndices.insert(it->edge());
        }
        
        // for all affected edges:
        for(std::set<std::size_t>::const_iterator it = affectedEdgeIndices.begin();
        it != affectedEdgeIndices.end(); ++it) { 
            // remove adjacencies
            eraseAdjacenciesForEdge(*it);

            // adapt vertex labels
            for(std::size_t j=0; j<2; ++j) {
                if(edges_[*it][j] == movingVertexIndex) {
                    edges_[*it][j] = vertexIndex;
                }
            }
            // if(!(edges_[*it].directedness()) && edges_[*it][0] > edges_[*it][1]) {
            if(edges_[*it][0] > edges_[*it][1]) {
                std::swap(edges_[*it][0], edges_[*it][1]);
            }
        }

        // move vertex
        vertices_[vertexIndex] = vertices_[movingVertexIndex]; // copy
        vertices_.pop_back(); // erase

        // insert adjacencies for edges of moved vertex
        for(std::set<std::size_t>::const_iterator it = affectedEdgeIndices.begin();
        it != affectedEdgeIndices.end(); ++it) { 
            insertAdjacenciesForEdge(*it);
        }

        visitor_.eraseVertex(vertexIndex);
        visitor_.relabelVertex(movingVertexIndex, vertexIndex);
    }
}

/// Erase an edge.
///
/// \param edgeIndex Integer index of the edge to be erased.
/// 
template<typename connectorData, typename nodeData, typename Visitor>
inline void 
Graph<connectorData, nodeData, Visitor>::eraseEdge(
    const std::size_t edgeIndex
) {
    assert(edgeIndex < numberOfEdges()); 

    eraseAdjacenciesForEdge(edgeIndex);
    if(edgeIndex == numberOfEdges() - 1) { // if the last edge is erased
        edges_.pop_back(); // delete
        visitor_.eraseEdge(edgeIndex);
    }
    else { 
        std::size_t movingEdgeIndex = numberOfEdges() - 1;
        eraseAdjacenciesForEdge(movingEdgeIndex);
        edges_[edgeIndex] = edges_[movingEdgeIndex]; // copy
        edges_.pop_back(); // delete
        insertAdjacenciesForEdge(edgeIndex);
        visitor_.eraseEdge(edgeIndex);
        visitor_.relabelEdge(movingEdgeIndex, edgeIndex);
    }
}

/// Search for an edge (in logarithmic time).
///
/// \param vertex0 first vertex of the edge.
/// \param vertex1 second vertex of the edge.
/// \return if an edge from vertex0 to vertex1 exists, pair.first is true 
///     and pair.second is the index of such an edge. if no edge from vertex0
///     to vertex1 exists, pair.first is false and pair.second is undefined.
///
// template<typename connectorData, typename nodeData, typename Visitor>
// inline std::pair<bool, std::size_t>
// Graph<connectorData, nodeData, Visitor>::findEdge(
//     const std::size_t vertex0,
//     const std::size_t vertex1
// ) const {
//     assert(vertex0 < numberOfVertices());
//     assert(vertex1 < numberOfVertices());

//     std::size_t v0 = vertex0;
//     std::size_t v1 = vertex1;
//     if(numberOfEdgesFromVertex(vertex1) < numberOfEdgesFromVertex(vertex0)) {
//         v0 = vertex1;
//         v1 = vertex0;
//     }
//     VertexIterator it = std::lower_bound(
//         verticesFromVertexBegin(v0),
//         verticesFromVertexEnd(v0),
//         v1
//     ); // binary search
//     if(it != verticesFromVertexEnd(v0) && *it == v1) {
//         // access the corresponding edge in constant time
//         const std::size_t j = std::distance(verticesFromVertexBegin(v0), it);
//         return std::make_pair(true, edgeFromVertex(v0, j));
//     }
//     else {
//         return std::make_pair(false, 0);
//     }
// }

#endif //GRAPH_HPP
