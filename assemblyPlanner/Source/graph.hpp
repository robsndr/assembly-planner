#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <exception>
#include <memory>
#include "node.hpp"
#include "visitor.hpp"


template<typename Visitor = VerboseGraphVisitor<std::size_t> >
class Graph{
public:
    // Construction
    Graph(const Visitor& = Visitor());
    Graph(const std::size_t, const std::size_t, const Visitor& = Visitor());

    // General Information
    std::size_t numberOfNodes() const;
    std::size_t numberOfEdges() const;
    std::size_t numberOfEdgesFromNode(const std::size_t);
    std::size_t numberOfEdgesToNode(const std::size_t);

    // Access specific nodes/vertices.
    Edge * edgeFromNode(const std::size_t, const std::size_t) const;
    Edge * edgeToNode(const std::size_t, const std::size_t) const;
    std::vector<Node*>& nodesFromNode(const std::size_t, const std::size_t) const;
    std::vector<Node*>& nodesToNode(const std::size_t, const std::size_t) const;
    // std::pair<bool, std::size_t> findEdge(const std::size_t, const std::size_t) const;

    // manipulation
    // inserttion
    Node* insertNode( const std::size_t, const NodeData);
    std::size_t insertNodes(const std::vector<Node*>& );
    std::size_t insertEdge(  const EdgeData, const std::size_t , const std::size_t);
    std::size_t insertEdges( const EdgeData, 
                             const std::size_t , 
                             const std::vector<std::size_t> & );
    
    // manipulation
    // removal
    bool eraseNode( const std::size_t); 
    bool eraseEdge( const std::size_t, std::size_t );
    std::pair<bool, std::size_t> findEdge( const std::size_t, const std::size_t );

    // TODO: implement as friend. For template might be complications.
    void print();
    void reset(); 

private:

    std::size_t findEdgeIndexHelper( Edge * );

    std::unordered_map< std::size_t, Node* > nodes_;
    std::vector< Edge* > edges_;
    Visitor visitor_;

    // bool checkNode(const std::size_t) const;
};

/* Construct a graph
    @param visitor: Visitor to follow changes of integer indices of vertices and edges.
**/
template<typename Visitor >
inline 
Graph<Visitor>::Graph(
    const Visitor& visitor
)
:   nodes_(),
    edges_(),
    visitor_(visitor)
{}

/* Construct a graph with preallocating memory for given Edges.
    @numberOfVertices: Number of vertices.
    @numberOfEdges: Number of edges.
    @visitor: Visitor to follow changes of integer indices of vertices and edges.
**/
template<typename Visitor>
inline 
Graph<Visitor>::Graph(
    const std::size_t numberOfNodes,
    const std::size_t numberOfEdges,
    const Visitor& visitor
)
:   nodes_(),
    edges_(),
    visitor_(visitor)
{
    // edges_.reserve(numberOfEdges);
    // visitor_.insertVertices("0", );
}
    
/* Get the number of nodes.
**/
template<typename Visitor>
inline std::size_t
Graph<Visitor>::numberOfNodes() const { 
    return nodes_.size(); 
}

/* Get the number of edges.
*/
template<typename Visitor>
inline std::size_t
Graph<Visitor>::numberOfEdges() const { 
    return edges_.size(); 
}

/* Get the number of edges that originate from a given node.
    @node string-id of a node.
**/
template<typename Visitor>
inline std::size_t
Graph<Visitor>::numberOfEdgesFromNode(
    const std::size_t node
) { 
    if ( nodes_.find(node) == nodes_.end() ) {
        std::cerr << std::endl << "Error: numberOfEdgesFromNode. Node " << node << " not in graph." << std::endl;
        return 0;
    }
    return nodes_[node]->numberOfSuccessors();
}

/* Get the number of edges that are incident to a given node.
    @node: string-id of a node.
**/
template<typename Visitor>
inline std::size_t
Graph<Visitor>::numberOfEdgesToNode(
    const std::size_t node
) { 
    if ( nodes_.find(node) == nodes_.end() ) {
        std::cerr << std::endl << "Error: numberOfEdgesFromNode. Node " << node << " not in graph." << std::endl;
        return 0;
    }
    return nodes_[node]->numberOfPredecessors();
}

/* Get the pointer to the j`th edge that originates from a given node.
    @node: string-id of a node.
    @j; number of edge. Between 0 and numberOfedgesFromNode(node) - 1.
**/
template<typename Visitor>
inline Edge*
Graph<Visitor>::edgeFromNode(
    const std::size_t node,
    const std::size_t j
) const {
    if ( nodes_.find(node) == nodes_.end() ) {
        std::cerr << std::endl << "Error: edgeFromNode. Node " << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }
    return nodes_[node]->children[j];
}

/* Get the pointer to the j`th edge that is incident to a given node.
    @node: string-id of a node.
    @j: index of edge. Between 0 and numberOfedgesToNode(node) - 1.
**/
template<typename Visitor>
inline Edge*
Graph<Visitor>::edgeToNode(
    const std::size_t node,
    const std::size_t j
) const {
    if ( nodes_.find(node) == nodes_.end() ) {
        std::cerr << std::endl << "Error: edgeToNode. Node " << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }
    return nodes_[node]->parents[j];
}

/* Get pointers to the nodes reachable from a given node via a specified edge.
    @node: string-id of a node.
    @j: index of the edge being used. Between 0 and numberOfedgesFromNode(node) - 1.
**/
template<typename Visitor>
inline std::vector<Node*> &
Graph<Visitor>::nodesFromNode(
    const std::size_t node,
    const std::size_t j
) const {
    if ( nodes_.find(node) == nodes_.end() ) {
        std::cerr << std::endl << "Error: nodesFromNode. Node " << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }
    return nodes_[node]->children[j].getSuccessors();
}

/* Get pointers to the nodes incident to a given node via a specified edge.
    @node: string-id of a node.
    @j: index of the edge being used. Between 0 and numberOfedgesFromNode(node) - 1.
**/
template<typename Visitor>
inline std::vector<Node*> &
Graph<Visitor>::nodesToNode(
    const std::size_t node,
    const std::size_t j
) const {
    if ( nodes_.find(node) == nodes_.end() ) {
        std::cerr << std::endl << "Error: nodesToNode. Node " << node << " not in graph." << std::endl;
        throw std::range_error("Unable to access node.");
    }
    return nodes_[node]->parents[j].getPredecessors();
}

/* Insert an additional Node.
    @node: string-id of the newly inserted node.
    @data: data asociated with the created node.
    \return: number of nodes present in the graph.
**/
template<typename Visitor>
inline Node *
Graph<Visitor>::insertNode(std::size_t nodeId , NodeData data) {
 Node * tempNode = new Node(nodeId, data);
    nodes_.insert(std::make_pair(nodeId ,tempNode));
    // visitor_.insertVertex(nodeId);
    return tempNode;
}

/* Insert multiple additional Nodes.
    @nodes: vector containing nodes to be inserted into the graph.
    \return: number of nodes present in the graph.
**/
template<typename Visitor>
inline std::size_t
Graph<Visitor>::insertNodes(
    const std::vector<Node*>& nodes
) {
    for(auto const node: nodes){
        nodes_.insert(std::make_pair(node->id_, node));
    }
    std::size_t position = nodes_.size();
    // for(const auto nd: nodes){
    //     visitor_.insertNodes(nd->id_, nodes.size());
    // }
    return position;
}

/* Insert additional edge.
    @srcNodeId: string-ids of the source nodes of the edges.
    @destNodeId: string-ids of the destinaion nodes of the edges.
    \return Integer index of the newly inserted edge.
**/ 
template<typename Visitor>
std::size_t
Graph<Visitor>::insertEdge(
    const EdgeData data,
    const std::size_t srcNodeId,
    const std::size_t destNodeId
) {

    if ( nodes_.find(srcNodeId) == nodes_.end() ) {
        std::cerr << "Unable to create edge. " 
                    << "Node " << srcNodeId << " not in graph." << std::endl;
        throw std::range_error("Unable to create edge.");
    }
    if ( nodes_.find(destNodeId) == nodes_.end() ) {
        std::cerr << "Unable to create edge. " 
                    << "Node" << destNodeId << " not in graph." << std::endl;
        throw std::range_error("Unable to create edge.");
    }

    Edge *edge = new Edge(data);
    edge->setDestination(nodes_[destNodeId]);
    edge->setSource(nodes_[srcNodeId]);
    edges_.push_back(edge);
    nodes_[srcNodeId]->addSuccessor(edge);
    nodes_[destNodeId]->addPredecessor(edge);

    return edges_.size();
}

/* Insert additional edges.
    @srcNodeId: string-ids of the source nodes of the edges.
    @destNodeId: string-ids of the destinaion nodes of the edges.
    \return Integer index of the newly inserted edge.
**/ 
template<typename Visitor>
inline std::size_t
Graph<Visitor>::insertEdges(
    const EdgeData data,
    const std::size_t srcNodeId,
    const std::vector<std::size_t> & destNodeId
) {
    for(auto const& dst: destNodeId) {
        insertEdge(data, srcNodeId, dst);
    }
    return edges_.size();
}

//TODO improve search to log time.
/* Search for an edge (in LINEAR time!!!!).
    @nodeSrc: first vertex of the edge.
    @nodeDst: second vertex of the edge.
    \return if an edge from nodeSrc to nodeDst exists, pair.first is true 
     and pair.second is the index of such an edge. if no edge from nodeSrc
     to nodeDst exists, pair.first is false and pair.second is undefined.
*/
template<typename Visitor>
std::pair<bool, std::size_t>
Graph<Visitor>::findEdge(
    const std::size_t nodeSrc, 
    const std::size_t nodeDst
) {

    // check if given nodes exist in the graph.
    if ( nodes_.find(nodeSrc) == nodes_.end() ) {
        std::cerr << "Edge search: source node does not exist in graph. " 
                    << "Node " << nodeSrc << " not in graph." << std::endl;
        return std::make_pair(false, 0);
    } 

    if ( nodes_.find(nodeDst) == nodes_.end() ) {
        std::cerr << "Edge search: destination node does not exist in graph. " 
                    << "Node " << nodeDst << " not in graph." << std::endl;
        return std::make_pair(false, 0);
    }

    // search the edges for the given connection. 
    //TODO: improve this step such that search is log=time instead linear.
    bool success = false;
    std::size_t edge_index;

    for(size_t j = 0; j < edges_.size(); j++){
        if(edges_[j]->getSource() == nodes_[nodeSrc] && edges_[j]->getDestination() == nodes_[nodeDst]){
            success = true;
            edge_index = j;
            break;
        }
    }
    
    return std::make_pair(success, edge_index);
}

/* Find the index corresponding to a given edge pointer.
    @edge: pointer to edge edge which index to find.
*/ 
template<typename Visitor>
inline std::size_t 
Graph<Visitor>::findEdgeIndexHelper( Edge * edge){
    std::size_t edge_index;
    for(size_t j = 0; j < edges_.size(); j++){
        if(edges_[j] == edge ){
            edge_index = j;
            break;
        }
    }
    return edge_index;
}


/* Erase an edge.
    @edgeIndex: Integer index of the edge to be erased.
*/ 
template<typename Visitor>
bool 
Graph<Visitor>::eraseEdge(
    const std::size_t nodeSrc, 
    const std::size_t nodeDst
) {
    // check if given nodes exist in the graph.
    if ( nodes_.find(nodeSrc) == nodes_.end() ) {
        std::cerr << "Edge search: source node does not exist in graph. " 
                    << "Node " << nodeSrc << " not in graph." << std::endl;
        return false;
    } 

    if ( nodes_.find(nodeDst) == nodes_.end() ) {
        std::cerr << "Edge search: destination node does not exist in graph. " 
                    << "Node " << nodeDst << " not in graph." << std::endl;
        return false;
    }

    std::pair<bool, std::size_t>  result = findEdge( nodeSrc, nodeDst);
    std::size_t edgeIndex= std::get<1>(result);

    if(!std::get<0>(result)){
        std::cerr << "eraseEdge: Specified edge is not present in the graph." << std::endl;
        std::cerr << "eraseEdge: Edge was not removed." << std::endl;
        return false;
    }

    Node* source_node = nodes_[nodeSrc];
    Node* destination_node = nodes_[nodeDst];
    source_node->removeSuccessor(nodeDst);
    destination_node->removePredecessor(nodeSrc);

    delete edges_[edgeIndex];
    edges_.erase(edges_.begin() + edgeIndex);    
    
    return true;
    // visitor_.eraseEdge(edgeIndex);
    // visitor_.relabelEdge(movingEdgeIndex, edgeIndex);
    
}

/* Erase a Node and all edges concerning this Node.
    @nodeId Integer index of the vertex to be erased.
**/ 
template<typename Visitor>
bool
Graph<Visitor>::eraseNode(
    const std::size_t nodeId
) {
    if ( nodes_.find(nodeId) == nodes_.end() ) {
        std::cerr << "Unable to find node to remove. " 
                  << "Node: " << nodeId << " not in graph. Was not removed." 
                  << std::endl;
        return false;
    }

    Node* node_to_remove = nodes_[nodeId];
    nodes_.erase(nodeId);

    std::vector<Edge*> predecessors = node_to_remove->getPredecessors();
    std::vector<Edge*> successors = node_to_remove->getSuccessors();
    std::size_t temp_edge_index;

    for(size_t j = 0; j < predecessors.size(); j++){
        Edge* edge_to_remove = predecessors[j];
        Node* predecessor_node = edge_to_remove->getSource();
        predecessor_node->removeSuccessor(nodeId);
        
        temp_edge_index = findEdgeIndexHelper(edge_to_remove);
        edges_.erase(edges_.begin() + temp_edge_index); 
        delete edge_to_remove;
    }

    for(size_t j = 0; j < successors.size(); j++){
        Edge* edge_to_remove = successors[j];
        Node* successor_node = edge_to_remove->getDestination();
        successor_node->removePredecessor(nodeId);
        
        temp_edge_index = findEdgeIndexHelper(edge_to_remove);
        edges_.erase(edges_.begin() + temp_edge_index); 
        delete edge_to_remove;
    }

    
    delete node_to_remove;

    return true;
}

/* DEBUG: print graph.
*/ 
template<typename Visitor>
void
Graph<Visitor>::print() 
{
    std::cout << std::endl << std::endl;
    std::cout << "************************  Current Node configuration  ************************" << std::endl;
    std::cout << "Number of Nodes in Graph: " << numberOfNodes()
              << "                   Number of Edges in Graph: " << numberOfEdges() << std::endl << std::endl;
    for (auto const& x : nodes_){
        if(x.second->data_.marked)
            x.second->print();
    }
    // std::cout << "Current Edge configuration:" << std::endl;
    // for (auto const& x : edges_){
    //     x->print();
    // }
    // std::cout << "************************r************************r************************" << std::endl;
    // std::cout << std::endl << std::endl;
}


template<typename Visitor>
void
Graph<Visitor>::reset() 
{
    for (auto const& x : nodes_){
        if(x.second->data_.marked)
            x.second->data_.reset();
    }
}


#endif //GRAPH_HPP
