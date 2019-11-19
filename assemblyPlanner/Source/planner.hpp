#ifndef PLANNERH_HPP
#define PLANNERH_HPP


#include <iostream>
#include <unordered_map>
#include "aostar.hpp"
#include "dotwriter.hpp"

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

    // search_graph = new Graph<>();

    BFS bfs_graph_converter;
    Graph<> * tree = bfs_graph_converter.run(root);
    Node * tree_root = tree->root_;

    NodeExpander expander(tree, costs_);

    int a = 1;
    
    while(a != 0){
        
        AOStarSearch aostar;
        AOStarState state = aostar(tree_root, expander);

        for (auto node : state.solution_sequence){
            std::cout << "Action: " << node->data_.name << "     Worker: " << node->data_.worker << std::endl;
        }
        

        std::cout << "Run next iteration? " << std::endl;
        std::cin >> a; 
    }

    DotWriter writer("tree.dot");
    tree->print(writer);

}

#endif