#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
//TODO: Introduce Smart-Pointer instead.


/*Forward declaration of Node so Conector knows about it
**/
template <class TypeNode, class TypeEdge> 
class Node;


/* Edge data class. Represens the interconnections within the graph.
**/
template <class TypeNode, class TypeEdge> 
class Edge{
public:
    Edge( TypeEdge );

    Edge( TypeEdge, 
          Node<TypeNode, TypeEdge> *, 
          Node<TypeNode, TypeEdge> *
        );

    Node<TypeNode, TypeEdge>* getDestination() const;
    Node<TypeNode, TypeEdge>* getSource() const;

    void setDestination( Node<TypeNode, TypeEdge>* );
    void setSource( Node<TypeNode, TypeEdge>* );

    std::size_t id_;
private:
    TypeEdge data_;
    Node<TypeNode, TypeEdge>* to_;
    Node<TypeNode, TypeEdge>* from_;
};

/*
    Class representing the Nodes within the graph. 
**/
template <class TypeNode, class TypeEdge>  
Edge<TypeNode, TypeEdge>::Edge( 
    TypeEdge data
) {
    data_ = data;
}

/*
    Class representing the Nodes within the graph. 
**/
template <class TypeNode, class TypeEdge>  
Edge<TypeNode, TypeEdge>::Edge(
    TypeEdge data, 
    Node<TypeNode, TypeEdge> * source, 
    Node<TypeNode, TypeEdge> * sink
) {
    data_ = data;
    from_ = source;
    to_   = sink;
}

/*
    Class representing the Nodes within the graph. 
**/
template <class TypeNode, class TypeEdge> 
inline Node<TypeNode, TypeEdge>*  
Edge<TypeNode,TypeEdge>::getDestination() const{
    return to_;
}

/*
    Class representing the Nodes within the graph. 
**/
template <class TypeNode, class TypeEdge> 
inline Node<TypeNode, TypeEdge>* 
Edge<TypeNode,TypeEdge>::getSource() const{
    return from_;
}

template <class TypeNode, class TypeEdge> 
inline void
Edge<TypeNode,TypeEdge>::setDestination(Node<TypeNode, TypeEdge>* node){
    to_ = node;
}

/*
    Class representing the Nodes within the graph. 
**/
template <class TypeNode, class TypeEdge> 
inline void
Edge<TypeNode,TypeEdge>::setSource(Node<TypeNode, TypeEdge>* node){
    from_ = node;
}


/*
    Class representing the Nodes within the graph. 
    It generalizes to AND/OR graphs so Connectors are used instead of edges.
    Connectors can be seen as k-edges. One connector can reach up to k destination nodes.
**/
template <class TypeNode, class TypeEdge> 
class Node{
public:
    Node(std::size_t identifier, TypeNode data);

    bool hasSuccessor() const;
    bool hasPredecessor() const;
    std::size_t numberOfSuccessors() const;
    std::size_t numberOfPredecessors() const;

    void addPredecessor(Edge<TypeNode,TypeEdge> *);
    void addSuccessor(Edge<TypeNode,TypeEdge> *);
    void removePredecessor(std::size_t);
    void removeSuccessor(std::size_t);
    Edge<TypeNode, TypeEdge>* getSuccessor(std::size_t) const;    
    Edge<TypeNode, TypeEdge>* getPredecessor(std::size_t) const;
    std::vector<Edge<TypeNode, TypeEdge>*> getSuccessors();    
    std::vector<Edge<TypeNode, TypeEdge>*> getPredecessors();


    bool visited = false;
    std::size_t id_;

private:


    std::unordered_map< std::size_t, Edge<TypeNode, TypeEdge>* > parents_;
    std::unordered_map< std::size_t, Edge<TypeNode, TypeEdge>* > children_; 
    TypeNode data_;
};

/* Constructor. 
    @identifier: unique id which is tied to a given node.
    @data: data container of Node. Type specified by user.
**/
template <class TypeNode, class TypeEdge> 
Node<TypeNode, TypeEdge>::Node(std::size_t identifier, TypeNode data){
    id_ = identifier;
    data_ = data;
}

/* Check if node has successors attached to it.
**/
template <class TypeNode, class TypeEdge> 
inline bool 
Node<TypeNode, TypeEdge>::hasSuccessor (void) const{
    return children_.empty();
}

/* Check if node has predecessors attached to it.
**/
template <class TypeNode, class TypeEdge> 
inline bool 
Node<TypeNode, TypeEdge>::hasPredecessor (void) const{
    return parents_.empty();
}

/* Obtain successor edge connecting the node to the one specified by the index-id.
    @index: node id. 
    \return: pointer to the edge connecting given node to the one specified by @index.
**/
template <class TypeNode, class TypeEdge> 
inline Edge<TypeNode, TypeEdge>* 
Node<TypeNode, TypeEdge>::getSuccessor(std::size_t index) const{
    return children_[index];
}

/* Obtain predecessor edge connecting the node to the one specified by the index-id.
    @index: node id. 
    \return: pointer to the edge connecting given node to the one specified by @index.
**/
template <class TypeNode, class TypeEdge> 
inline Edge<TypeNode, TypeEdge>* 
Node<TypeNode, TypeEdge>::getPredecessor(std::size_t index) const{
    return parents_[index];
}

/* Add provided edge to the set of predecessors of Node.
    @predecessor: pointer to the Edge-object which should be added as predecessor of Node.
**/
template <class TypeNode, class TypeEdge> 
inline void 
Node<TypeNode, TypeEdge>::addPredecessor(
    Edge<TypeNode, TypeEdge> * predecessor
) {
    parents_.insert(std::make_pair(predecessor->getSource()->id_, predecessor));
}

/* Add provided edge to the set of successors of Node.
    @successor: pointer to the Edge-object which should be added as successor of Node.
**/
template <class TypeNode, class TypeEdge> 
inline void 
Node<TypeNode, TypeEdge>::addSuccessor(
    Edge<TypeNode, TypeEdge> * successor
) {
    children_.insert(std::make_pair(successor->getDestination()->id_, successor));
}

/* Remove Edge connecting the Node with the one specified by @predId from the set of predecessors.
    @predId: id of node specifying the Edge to delete.
**/
template <class TypeNode, class TypeEdge> 
inline void 
Node<TypeNode, TypeEdge>::removePredecessor(
    std::size_t predId
) {
    parents_.erase(predId);
}

/* Remove Edge connecting the Node with the one specified by @succId from the set of successors.
    @succId: id of node specifying the Edge to delete.
**/
template <class TypeNode, class TypeEdge> 
inline void 
Node<TypeNode, TypeEdge>::removeSuccessor(
    std::size_t succId
) {
    children_.erase(succId);
}

/* Get the number of successors connected to the Node.
    \return: number of successors.
**/
template <class TypeNode, class TypeEdge> 
inline std::size_t 
Node<TypeNode, TypeEdge>::numberOfSuccessors( ) const {
    return children_.size();
}

/* Get the number of predecessors connected to the Node.
    \return: number of predecessors.
**/
template <class TypeNode, class TypeEdge> 
inline std::size_t
Node<TypeNode, TypeEdge>::numberOfPredecessors() const{
    return parents_.size();
}

/* Obtain predecessor edge connecting the node to the one specified by the index-id.
    @index: node id. 
    \return: pointer to the edge connecting given node to the one specified by @index.
**/
template <class TypeNode, class TypeEdge> 
inline std::vector<Edge<TypeNode, TypeEdge>*>  
Node<TypeNode, TypeEdge>::getSuccessors() {
    std::vector<int> vecOfValues;
    vecOfValues.reserve(children_.size());
 
    /*** Copy all value fields from map to a vector using Lambda function ***/
    std::for_each(children_.begin(), children_.end(),  [&](std::pair<std::size_t, Edge<TypeNode, TypeEdge>*>  & element){
													vecOfValues.push_back(element.second);
												});
}

/* Obtain predecessor edge connecting the node to the one specified by the index-id.
    @index: node id. 
    \return: pointer to the edge connecting given node to the one specified by @index.
**/
template <class TypeNode, class TypeEdge> 
inline std::vector<Edge<TypeNode, TypeEdge>*>  
Node<TypeNode, TypeEdge>::getPredecessors() {
    std::vector<int> vecOfValues;
    vecOfValues.reserve(children_.size());
 
    /*** Copy all value fields from map to a vector using Lambda function ***/
    std::for_each(children_.begin(), children_.end(),  [&](std::pair<std::size_t, Edge<TypeNode, TypeEdge>*>  & element){
													vecOfValues.push_back(element.second);
												});
}