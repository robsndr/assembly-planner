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
    Node<std::size_t,std::size_t> * pointr1 = new Node<std::size_t,std::size_t>(1,0);
    Node<std::size_t,std::size_t> * pointr2 = new Node<std::size_t,std::size_t>(2,0);
    Node<std::size_t,std::size_t> * pointr3 = new Node<std::size_t,std::size_t>(3,0);
    Node<std::size_t,std::size_t> * pointr4 = new Node<std::size_t,std::size_t>(4,0);
    Node<std::size_t,std::size_t> * pointr5 = new Node<std::size_t,std::size_t>(5,0);
    Node<std::size_t,std::size_t> * pointr6 = new Node<std::size_t,std::size_t>(6,0);
    Node<std::size_t,std::size_t> * pointr7 = new Node<std::size_t,std::size_t>(7,0);
    Node<std::size_t,std::size_t> * pointr8 = new Node<std::size_t,std::size_t>(8,0);
    Node<std::size_t,std::size_t> * pointr9 = new Node<std::size_t,std::size_t>(9,0);
    Node<std::size_t,std::size_t> * pointr10 = new Node<std::size_t,std::size_t>(10,0);

    vec.push_back(pointr1);
    vec.push_back(pointr2);
    vec.push_back(pointr3);
    vec.push_back(pointr4);
    vec.push_back(pointr5);
    vec.push_back(pointr6);

    graph.insertNodes(vec);

    std::size_t src = 1;
    std::vector<std::size_t>  dst;
    dst.push_back(src);
    graph.insertEdge(0, 1, 5);
    graph.insertEdge(0, 4, 5);
    graph.insertEdge(0, 4, 2);
    graph.insertEdge(0, 1, 2);
    graph.insertEdge(0, 5, 2);
    graph.insertEdge(0, 4, 1);
    graph.insertEdge(0, 3, 4);
    graph.insertEdge(0, 1, 4);




    std::cout << "Edges from Node '1': "<< graph.numberOfEdgesFromNode(1) << std::endl;
    std::cout << "Number of Nodes in Graph: " << graph.numberOfNodes() << std::endl;
    std::cout << "Number of Edges in Graph: " << graph.numberOfEdges() << std::endl;


    std::cout   << "Number of Edges from Node '4': " 
                 << graph.numberOfEdgesFromNode(4) 
                << std::endl;

    std::cout   << "Number of Edges to Node '4': " 
                << graph.numberOfEdgesToNode(4)
                << std::endl;               
    
    // graph.eraseEdge(4,1);
    // graph.eraseEdge(3,4);

    std::cout << "***********************ERase 4**************************************************" << std::endl;         

    graph.eraseNode(4);

    std::cout << "Edges from Node '1': "<< graph.numberOfEdgesFromNode(1) << std::endl;
    std::cout << "Number of Nodes in Graph: " << graph.numberOfNodes() << std::endl;
    std::cout << "Number of Edges in Graph: " << graph.numberOfEdges() << std::endl;

    std::cout   << "Number of Edges from Node '4': " 
                 << graph.numberOfEdgesFromNode(4) 
                << std::endl;

    std::cout   << "Number of Edges to Node '4': " 
                << graph.numberOfEdgesToNode(4)
                << std::endl;   

    return 0;
}