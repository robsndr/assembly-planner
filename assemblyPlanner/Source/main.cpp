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

    Graph<std::string,std::string> graph; 
    std::cout << "Number of Nodes: " << graph.numberOfNodes() << std::endl;
    graph.insertNode("ab", "1");
    std::cout << "Number of Nodes: " << graph.numberOfNodes() << std::endl;

    std::vector<Node<std::string,std::string>*> vec;
    Node<std::string,std::string> * pointr = new Node<std::string,std::string>("qaz","4");
    Node<std::string,std::string> * pointr2 = new Node<std::string,std::string>("wsx","5");

    vec.push_back(pointr);
    vec.push_back(pointr2);
    graph.insertNodes(vec);

    std::string src("qaz");
    std::vector<std::string>  dst;
    dst.push_back("wsx");
    graph.insertEdge("1", src, "wsx");



    std::cout << "Edges from Node: "<< graph.numberOfEdgesFromNode("ab") << std::endl;
    std::cout << "Number of Nodes: " << graph.numberOfNodes() << std::endl;
    std::cout << "Number of Edges: " << graph.numberOfEdges() << std::endl;

    std::cout   << "Number of Edges from Node 'qaz': " 
                << graph.numberOfEdgesFromNode("qaz") 
                << std::endl;

    std::cout   << "Number of Edges to Node 'qaz': " 
                << graph.numberOfEdgesToNode("qaz")
                << std::endl;

    std::cout   << "Number of Edges to Node 'wsx': " 
                << graph.numberOfEdgesToNode("wsx")
                << std::endl;

    return 0;
}