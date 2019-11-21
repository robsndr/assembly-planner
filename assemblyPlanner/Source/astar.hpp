#pragma once

#include <iostream>
#include <climits>
#include <set>
#include <vector>
#include <queue>
#include "expander.hpp"

class AStarSearch{
private:
    /* data */
public:
    AStarSearch(/* args */);
    ~AStarSearch();
    void search(Graph<> *, Node * , NodeExpander *);
};

AStarSearch::AStarSearch(/* args */){
}

AStarSearch::~AStarSearch(){
}


void AStarSearch::search(Graph<> * graph, Node* root, NodeExpander * expander){
    
    Node *current = nullptr;

    auto cmp = [](Node* left, Node* right) { return (left->data_.f_score ) < (right->data_.f_score); };
    std::priority_queue<Node*, std::vector<Node* >, decltype(cmp)> openSet(cmp);

    // NodeSet closedSet;
    root->data_.calc_hscore();
    root->data_.calc_fscore(); 
    openSet.push(root);

    while (!openSet.empty()) {
        current = openSet.top();
        openSet.pop();

        if (current->data_.isGoal()) {
            return;
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


            // This step might not be needed because tree characteristic of graph.
            // if child.position is in the openList's nodes positions
            //     if the child.g is higher than the openList node's g
            //         continue to beginning of for loop
            // child->data_.f_score = child.g + child.h

            openSet.push(child);

        }
        
    }
    
}