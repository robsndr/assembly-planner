#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "graph.hpp"

int main(void){

    Graph<std::size_t,std::size_t> graph; 

    std::vector<Node<std::size_t,std::size_t>*> vec;
    Node<std::size_t,std::size_t> * pointr1 = new Node<std::size_t,std::size_t>(1,0);
    Node<std::size_t,std::size_t> * pointr2 = new Node<std::size_t,std::size_t>(2,0);
    Node<std::size_t,std::size_t> * pointr3 = new Node<std::size_t,std::size_t>(3,0);
    Node<std::size_t,std::size_t> * pointr4 = new Node<std::size_t,std::size_t>(4,0);
    Node<std::size_t,std::size_t> * pointr5 = new Node<std::size_t,std::size_t>(5,0);
    Node<std::size_t,std::size_t> * pointr6 = new Node<std::size_t,std::size_t>(6,0);

    vec.push_back(pointr1);
    vec.push_back(pointr2);
    vec.push_back(pointr3);
    vec.push_back(pointr4);
    vec.push_back(pointr5);
    vec.push_back(pointr6);

    graph.insertNodes(vec);

    graph.insertEdge(0, 1, 5);
    graph.insertEdge(0, 4, 5);
    graph.insertEdge(0, 4, 2);
    graph.insertEdge(0, 1, 2);
    graph.insertEdge(0, 5, 2);
    graph.insertEdge(0, 4, 1);
    graph.insertEdge(0, 3, 4);
    graph.insertEdge(0, 1, 4);

    graph.print();
;        
    graph.eraseNode(4);

    graph.print();

    graph.eraseEdge(5,2);

    graph.print();

    return 0;
}