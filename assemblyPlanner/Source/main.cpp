#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "graph.hpp"

int main(void){

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

    std::vector<std::string>  src;
    std::vector<std::string>  dst;
    src.push_back("qaz");
    dst.push_back("wsx");
    graph.insertConnector("1", src, dst);

    std::cout << "Connectors from Node: "<< graph.numberOfConnectorsFromNode("ab") << std::endl;
    std::cout << "Number of Nodes: " << graph.numberOfNodes() << std::endl;
    std::cout << "Number of Edges: " << graph.numberOfConnectors() << std::endl;

    return 0;
}