#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "graph.hpp"

int main(void){

    // Connector<nodeData, connectorData> * connectorFromNode(const std::string, const std::size_t);
    // Connector<nodeData, connectorData> * connectorToNode(const std::string, const std::size_t);
    // std::vector<Node<nodeData, connectorData>*>& nodesFromNode(const std::string, const std::size_t);
    // std::vector<Node<nodeData, connectorData>*>& nodesToNode(const std::string, const std::size_t);

    Graph<std::size_t,std::size_t> graph; 
    std::cout << "Number of Nodes: " << graph.numberOfNodes() << std::endl;
    graph.insertNode(0, 0);
    std::cout << "Number of Nodes: " << graph.numberOfNodes() << std::endl;

    std::vector<Node<std::size_t,std::size_t>*> vec;
    Node<std::size_t,std::size_t> * pointr = new Node<std::size_t,std::size_t>(1,0);
    Node<std::size_t,std::size_t> * pointr2 = new Node<std::size_t,std::size_t>(2,0);

    vec.push_back(pointr);
    vec.push_back(pointr2);
    graph.insertNodes(vec);

    std::size_t src = 2;
    std::vector<std::size_t>  dst;
    dst.push_back(src);
    graph.insertEdge(1, src, 1);



    std::cout << "Edges from Node '1': "<< graph.numberOfEdgesFromNode(1) << std::endl;
    std::cout << "Number of Nodes in Graph: " << graph.numberOfNodes() << std::endl;
    std::cout << "Number of Edges in Graph: " << graph.numberOfEdges() << std::endl;

    std::cout   << "Number of Edges from Node '1': " 
                << graph.numberOfEdgesFromNode(1) 
                << std::endl;

    std::cout   << "Number of Edges to Node '1': " 
                << graph.numberOfEdgesToNode(1)
                << std::endl;

    std::cout   << "Number of Edges from Node '2': " 
                << graph.numberOfEdgesFromNode(2) 
                << std::endl;

    std::cout   << "Number of Edges to Node '2': " 
                << graph.numberOfEdgesToNode(2)
                << std::endl;                

    std::cout   << "Number of Edges to Node '2': " 
                << graph.numberOfEdgesToNode(2)
                << std::endl;

    graph.eraseEdge(1,2);
    graph.eraseEdge(2,1);

    std::cout << "*************************************************************************" << std::endl;

    std::cout << "Edges from Node '1': "<< graph.numberOfEdgesFromNode(1) << std::endl;
    std::cout << "Number of Nodes in Graph: " << graph.numberOfNodes() << std::endl;
    std::cout << "Number of Edges in Graph: " << graph.numberOfEdges() << std::endl;

    std::cout   << "Number of Edges from Node '1': " 
                << graph.numberOfEdgesFromNode(1) 
                << std::endl;

    std::cout   << "Number of Edges to Node '1': " 
                << graph.numberOfEdgesToNode(1)
                << std::endl;

    std::cout   << "Number of Edges from Node '2': " 
                << graph.numberOfEdgesFromNode(2) 
                << std::endl;

    std::cout   << "Number of Edges to Node '2': " 
                << graph.numberOfEdgesToNode(2)
                << std::endl;                

    std::cout   << "Number of Edges to Node '2': " 
                << graph.numberOfEdgesToNode(2)
                << std::endl;

    return 0;
}