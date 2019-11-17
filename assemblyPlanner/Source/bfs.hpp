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

private:

    std::string getUniqueName(std::string);
    std::unordered_map<std::string, std::size_t> name_index_;
};

Graph<> * BFS::run(Node * root){ 
  
    // Create a queue for BFS 
    std::queue<Node* > queue;
    std::queue<Node* > queue2; 
  
    queue.push(root); 

    // Create search tree.
    Graph<> * search_tree = new Graph<>;
    std::size_t node_count = 0;
    Node* new_root = search_tree->insertNode(node_count, root->data_);
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
            node_count++;

            node_data = x->data_;
            node_data.name = getUniqueName(node_data.name);
            queue2.push(search_tree->insertNode(node_count, node_data));
            search_tree->insertEdge(0, temp2->id_, node_count);

            queue.push(x);
        }
    }

    // search_tree->print();
    // Retrun root of newly created graph.
    return search_tree;
} 

/* Generates a unique identifier for a given node.
    As the graph is converted towards a tree, nodes can be repeated.
    Therefore a unique identifier for every node is needed.
**/
std::string BFS::getUniqueName(std::string node_name){
    
    std::unordered_map<std::string, std::size_t>::iterator it;
    it = name_index_.find(node_name);

    if (it != name_index_.end()){
        name_index_[node_name]++;
    }
    else{
        name_index_[node_name] = 0;
    }

    return node_name + "_x" + std::to_string(name_index_[node_name]);
}