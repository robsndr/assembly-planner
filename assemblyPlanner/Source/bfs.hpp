#ifndef BFSH_HPP
#define BFSH_HPP

#include <iostream>
#include <climits>
#include <set>
#include <vector>
#include "graph.hpp"
#include <queue>
#include <string>

class BFS{

public:
    BFS(){}

    Graph<> * run(Node * );

};

Graph<> * BFS::run(Node * root){ 
  
    // Create a queue for BFS 
    std::queue<Node* > queue;
    std::queue<Node* > queue2; 
  
    queue.push(root); 

    // Create search tree.
    Graph<> * search_tree = new Graph<>;
    Node* new_root = search_tree->insertNode(root->data_);
    search_tree->root_ = new_root;
    queue2.push(new_root);

    NodeData node_data;

    while(!queue.empty()){ 

        // Dequeue a vertex from queue
        Node* temp = queue.front();  
        queue.pop(); 

        Node* temp2 = queue2.front();  
        queue2.pop(); 

        // Insert node from original Graph to currently built tree.
        for (auto x : temp->getSuccessorNodes()){

            node_data = x->data_;
            // node_data.name = getUniqueName(node_data.name);

            Node * new_node = search_tree->insertNode(node_data);
            queue2.push(new_node);
            search_tree->insertEdge(0, temp2->id_, new_node->id_);

            queue.push(x);
        }
    }

    // search_tree->print();
    // Retrun root of newly created graph.
    return search_tree;
} 

#endif