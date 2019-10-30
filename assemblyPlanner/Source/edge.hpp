/*Forward declaration of Node so Conector knows about it
**/
#include <iostream>

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

    void print();

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
**/
template <class TypeNode, class TypeEdge> 
inline void
Edge<TypeNode,TypeEdge>::print(){
    std::cout << "Edge:  " << getSource()->id_ << " ---> " << getDestination()->id_ << std::endl;
}

