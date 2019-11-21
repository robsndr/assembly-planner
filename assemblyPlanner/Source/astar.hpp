#pragma once

#include <iostream>
#include <climits>
#include <set>
#include <vector>
#include <queue>
#include "expander.hpp"


struct LessThan
{
  bool operator()(const Node* lhs, const Node* rhs) const{
    return (lhs->data_.f_score ) > (rhs->data_.f_score);
  }
};

class AStarSearch{
private:
    /* data */
public:
    AStarSearch(/* args */);
    ~AStarSearch();
    Node* search(Graph<> *, Node * , NodeExpander *);
};

AStarSearch::AStarSearch(/* args */){
}

AStarSearch::~AStarSearch(){
}


Node * AStarSearch::search(Graph<> * graph, Node* root, NodeExpander * expander){
    
    Node *current = nullptr;

    // auto cmp = [](Node* left, Node* right) { return ; };
    std::priority_queue<Node*, std::vector<Node* >, LessThan> openSet;

    // NodeSet closedSet;
    root->data_.calc_hscore();
    root->data_.calc_fscore(); 
    openSet.push(root);

    while (!openSet.empty()) {
        current = openSet.top();
        openSet.pop();

        //  std::cout << "Current f_score: " << current->data_.f_score << std::endl;

        if (current->data_.isGoal()) {
            std::cout << "GOAL";
            return current;
        }

        expander->expandNode(current);

        // closedSet.insert(current);

        current->data_.marked = true;

        for (auto & edge : current->getSuccessors()){
            

            Node * child = edge->getDestination();

            if(child->data_.marked)
                continue;
            
            child->data_.g_score = current->data_.g_score + edge->data_.cost;
            child->data_.calc_hscore();
            child->data_.calc_fscore();
            // std::cout << "g_score " << edge->data_.cost << std::endl;
            // std::cout << "h_score " << child->data_.h_score << std::endl;
            // std::cout << "f_score " << child->data_.f_score << std::endl;




            // This step might not be needed because tree characteristic of graph.
            // if child.position is in the openList's nodes positions
            //     if the child.g is higher than the openList node's g
            //         continue to beginning of for loop
            // child->data_.f_score = child.g + child.h

            openSet.push(child);

            // std::cout << "SIZEE: " << openSet.size();
            
        }
        
    }
    return current;
}