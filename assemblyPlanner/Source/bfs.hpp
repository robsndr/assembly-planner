#include <iostream>
#include <climits>
#include <set>
#include <vector>
#include "graph.hpp"
#include <queue>

Node * BFS(Node * root){ 
  
    // Create a queue for BFS 
    std::queue<Node* > queue;
    std::queue<Node* > queue2; 
  
    queue.push(root); 

    // Create search tree.
    Graph<> * search_tree = new Graph<>;
    std::size_t node_count = 0;
    Node* new_root = search_tree->insertNode(node_count, root->data_);
    queue2.push(new_root);


    while(!queue.empty()){ 

        // Dequeue a vertex from queue
        Node* temp = queue.front();  
        queue.pop(); 

        Node* temp2 = queue2.front();  
        queue2.pop(); 

        // Insert node from original Graph to currently built tree.
        for (auto x : temp->getSuccessorNodes()){
            node_count++;

            queue2.push(search_tree->insertNode(node_count, x->data_));
            search_tree->insertEdge(0, temp2->id_, node_count);

            queue.push(x);
        }
    }

    // Retrun root of newly created graph.
    return new_root;
} 