#ifndef PLANNERH_HPP
#define PLANNERH_HPP


#include <iostream>
#include <unordered_map>
#include "dotwriter.hpp"
#include "astar.hpp"

class Planner{
public:
    Planner();
    ~Planner();
    
    void operator()(Graph<> *, Node*, CostMap &);

private:
    std::vector<Node *> nodes_to_expand;
    std::vector<Node *> ors;

    Graph<> * search_graph;

};

Planner::Planner(){}

Planner::~Planner(){}


void Planner::operator()(Graph<> * graph, Node* root, CostMap & costs_){


    search_graph = new Graph<>();
    Node * new_root = search_graph->insertNode(root->data_);
    new_root->data_.name = "";
    new_root->data_.subassemblies[root->data_.name] =  root;

    for (auto &x : root->getSuccessorNodes()){
        new_root->data_.actions[x->data_.name] = x;    
    }
     

    NodeExpander * expander = new NodeExpander(search_graph, costs_);

    // BFS bfs_graph_converter;
    // Graph<> * tree = bfs_graph_converter.run(root);
    // Node * tree_root = tree->root_;

    // Graph<> * tree = graph;s
    // Node * tree_root = root;


    int a = 1;
    
    while(a != 0){
        
        AStarSearch astar;
        astar.search(search_graph, new_root, expander);
        // AOStarState state = aostar(tree_root, expander);

        // for (auto node : state.solution_sequence){
        //     std::cout << "Action: " << node->data_.name << "     Worker: " << node->data_.worker << std::endl;
        // }
        

        std::cout << "Run next iteration? " << std::endl;
        std::cin >> a; 
    }

    DotWriter writer("tree.dot");
    // tree->print(writer);

}

#endif